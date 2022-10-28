"""
leaf_wrap.py

Creates a lightweight C++ wrapper for LEAF by analysing the header files.
"""

import os
import re

# The folder where the leaf headers are stored.
folder = '/Users/jjjlaw/Documents/GitHub/stm32-midi-keyboard/Core/Inc/leaf'

# File to output C++ symbols.
output_file = 'leaf.hpp'

# Leaf these files out of analysis.
blacklist_files = ['leaf.h', 'leaf-math.h', 'leaf-global.h', 'leaf-config.h', 'leaf-tables.h']

# Rename common subnamespaces.
namespace_renames = {
    'dynamics': 'dyn',
    'effects': 'fx',
    'envelopes': 'env',
    'instruments': 'inst',
    'oscillators': 'osc',
    'reverb': 'rev',
}

class_renames = {
    r'808_(\w*)': r'r808\1'
}

# Lines to add at the beginning of the file.
file_preamble = \
"""#ifndef LEAF_HPP
#define LEAF_HPP

#include "leaf.h"

namespace leaf
{
    extern LEAF main;

    inline void init(LEAF& leaf, float sampleRate, char* memory, size_t memorySize, float (*random)(void))
    {
        LEAF_init(&leaf, sampleRate, memory, memorySize, random);
    }

    inline void init(float sampleRate, char* memory, size_t memorySize, float (*random)(void))
    {
        init(main, sampleRate, memory, memorySize, random);
    }

    inline void setSampleRate(LEAF& leaf, float sampleRate) { LEAF_setSampleRate(&leaf, sampleRate); }
    inline void setSampleRate(float sampleRate) { setSampleRate(main, sampleRate); }
    inline float getSampleRate(const LEAF& leaf = main) { return leaf.sampleRate; }

"""

starting_indent = 1 # The indent to have after writing the preamble.

file_match = re.compile(r'leaf-(\w+)\.h')
func_match = re.compile(r'^([\w*_]+) ([\w_]+)\((.*)\);')

func_template = \
"""{ret} {fname}({args}) {{ {body} }}"""


def indent_lines(lines, indent=4):
    sp = ' ' * indent
    # return sp + ('\n' + sp).join(lines)
    return ['\n' + sp + line for line in lines]

def join_lines(lines, indent=4):
    return '\n'.join(indent_lines(lines, indent))

def make_id(typ, var, const=False, ref=False, ptr=False, init=None):
    const_str = ['', 'const '][const]
    ptr_str = ['', '*'][ptr]
    ref_str = ['', '&'][ref]
    init_str = f' = {init}' if init else ''
    return f'{const_str}{typ}{ptr_str}{ref_str} {var}{init_str}'


def make_function(ret, fname, args, body):
    return func_template.format(ret=ret, fname=fname, args=', '.join(make_id(**arg) for arg in args), body=body)


def leafname_to_snake(name: str) -> str:
    s = ''
    i = 0
    if name[0] == 't' and not name[1].islower():
        i += 1 # Skip leaf's 't' prefix.

    prev_upper = True
    while i != len(name):
        curr_upper = name[i].isupper()
        if curr_upper and not prev_upper:
            s += '_'
        s += name[i].lower()
        prev_upper = curr_upper
        i += 1
    
    return s


def process_func_decl(decl):
    res = func_match.match(decl)
    if not res:
        return

    is_class_based = False

    # Found a match.
    typ, og_fname, args = res.groups()
    args = [get_arg(s.strip()) for s in args.split(',')]

    fname = og_fname

    # Get class name.
    tokens = fname.split('_')
    if len(tokens) == 1:
        cname = None
    else:
        cname, fname = tokens[0], '_'.join(tokens[1:])
        # cname, fname = tokens[0], tokens[1:]
    
        # Check if first param is a class instance param.
        if args[0]['typ'].startswith(cname):
            is_class_based = True
            args = args[1:] # Remove it.

        # Convert LEAF's camelCase to snake_case.
        # cname = leafname_to_snake(cname)

    return is_class_based, typ, cname, og_fname, fname, args

def get_arg(arg):
    try:
        arg, init = arg.split('=')
    except ValueError:
        init = None

    arg = arg.strip()
    
    const = ' const' in arg # Simple, janky parser. But since the format of leaf is pretty simple, so janky is fine.
    ptr = '*' in arg
    if ptr and const:
        if arg.index(' const') > arg.index('*'):
            # Then const is after the ptr, so it doesn't really affect anything.
            const = False


    arg = arg.replace('*', '')
    arg = arg.replace('const', '') # Remove modifiers.
    try:
        *typ, arg = arg.split()
        assert len(typ) >= 1
        typ = ' '.join(typ)
    except (ValueError, AssertionError):
        typ, arg = arg.strip(), ''

    if not arg:
        arg = leafname_to_snake(typ) # No param name? Just use the type.
        
    return {
        'typ': typ,
        'var': arg,
        'const': const,
        'ptr': ptr,
        'init': init,
    }

class Writer:
    """
    Janky code for printing code line by line.
    """
    def __init__(self):
        self.indent = 0
        self.lines = []
        for line in file_preamble.splitlines():
            self.write(line)
        self.indent = starting_indent

    def finish(self):
        self.end_namespace()
        self.write()
        self.write('#endif')

    def write(self, *args, indent=0):
        if args:
            self.lines.append(((self.indent + indent) * 4) * ' ' + ' '.join(args))
        else:
            self.lines.append('')

    def begin_block(self, block, name):
        self.write(block, name)
        self.write('{')
        self.indent += 1
    
    def end_block(self, suffix=''):
        self.indent -= 1
        self.write('}' + suffix)
        self.write()
    
    def begin_namespace(self, name):
        self.begin_block('namespace', name)
    
    def end_namespace(self):
        self.end_block()

    def begin_class(self, name):
        self.begin_block('class', name)

    def begin_struct(self, name):
        self.begin_block('struct', name)

    def end_class(self):
        self.end_block(';')

    def save(self, file):
        with open(file, 'w') as f:
            f.write('\n'.join(self.lines))
    

def make_cpp_wrapper(out, file):
    prev_cname = ''
    ns = file_match.findall(file)[0]

    if ns in namespace_renames:
        ns = namespace_renames[ns]

    out.begin_namespace(ns)

    break_times = 0 # Count the number of line breaks between two methods of the same class. We want to preserve logical separations.

    with open(file) as f:
        for line in f.readlines():
            if not line.strip():
                break_times += 1
                continue
            
            try:
                is_class_based, typ, cname, og_fname, fname, args = process_func_decl(line)
            except TypeError:
                continue

            if cname is None:
                continue

            cname_snek = leafname_to_snake(cname)
            for pat, sub in class_renames.items():
                if re.match(pat, cname_snek):
                    cname_snek = re.sub(pat, sub, cname_snek)
                    break
            
            if cname_snek == fname:
                # Function names clash. Make function name distinct.
                fname += '_'

            if cname.lower() != prev_cname.lower():
                # New class name encountered. Close the class, and open a new one.
                if prev_cname:
                    out.end_class() # Class dismissed~

                if is_class_based:
                    out.begin_struct(cname_snek)
                    # out.write('public:', indent=-1)
                    out.write(f'{cname} obj;')
                    out.write()
                else:
                    out.begin_namespace(cname_snek)

                break_times = 0 # Reset for a new class.
            else:
                if break_times > 0:
                    for _ in range(break_times):
                        out.write()
                    break_times = 0

            # Cleanup arguments.
            for i, arg in enumerate(args):
                if arg['typ'] == 'LEAF' and arg['ptr']:
                    # Make leaf take a default (leaf main).
                    arg['ref'] = True
                    arg['ptr'] = False
                    if i == len(args) - 1:
                        arg['init'] = 'main'

                if arg['ptr'] and arg['var'].startswith('('):
                    # Function poontar!
                    arg['var'] = '(*' + arg['var'][1:] # Jank workaround for properly adding the pointer back for function pointers.
                    arg['ptr'] = False

            if is_class_based:
                apply_args = ['&obj'] # Pass in the class object.
            else:
                apply_args = []

            for arg in args:
                if arg['typ'] == 'LEAF':
                    apply_args += ['&' + arg['var']] # Handle LEAF object specially.
                else:
                    a = arg['var']
                    if '*' in arg['var']:
                        a = arg['var'][2 : arg['var'].index(')')] # Jank workaround for getting names of function pointers.
                    apply_args += [a]

            body = f'{og_fname}({", ".join(apply_args)});'
            if typ != 'void':
                body = 'return ' + body # Add `return` for non-void functions.

            func = make_function(ret=typ, fname=fname, args=args, body=body)
            if not is_class_based:
                func = 'inline ' + func

            out.write(func)

            prev_cname = cname

    out.end_class()
    out.end_namespace()


def is_leaf_file(filename):
    return filename.startswith('leaf-') and filename.endswith('.h')

if __name__ == '__main__':
    wrt = Writer()

    for file in os.listdir(folder):
        # Iterate through all files.
        if file.split('/')[-1] in blacklist_files:
            continue
        if is_leaf_file(file):
            make_cpp_wrapper(wrt, folder + '/' + file)

    wrt.finish()
    wrt.save('leaf.hpp')
