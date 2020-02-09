#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""


import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

    def choosenlines(self, k):
        return random.sample(self.lines, k)

    def length_f(self):
        return len(self.lines)
            
def main():
    version_msg = "%prog 2.0"
    usage_msg = """python3 shuf.py [OPTION]... [FILE]
  or:  python3 shuf.py -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n","--head-count", action="store",\
                      dest="numlines", help='output at most COUNT lines')
    parser.add_option("-i","--input-range",action="store",\
                      dest="i_range",\
                      help="treat each number LO through HI as an input line", \
                      default="NULL")
    parser.add_option("-r","--repeat", action="store_true", \
                      dest="rep",\
                      help='output lines can be repeated', default= False)
    
    
    options, args = parser.parse_args(sys.argv[1:])

    o_file = False
    int_c = -1

    try:
        if options.numlines:
            try:
                numlines = int(options.numlines)
            except:
                parser.error("invalid NUMLINES: {0}".
                         format(options.numlines))
            if numlines < 0:
                parser.error("negative count: {0}".
                         format(numlines))
            int_c = numlines

        elif not options.rep:
            int_c = -2

        if options.i_range == "NULL":
            if len(args) == 0:
                iList = sys.stdin.readlines()
                max_count= len(iList)
                o_file = True
            elif len(args) != 1:
                parser.error("wrong number of operands")
            else:
                if args[0] == '-':
                    iList = sys.stdin.readlines()
                    max_count= len(iList)
                    o_file = True
                else:
                    input_file = args[0]
                    try:
                        generator = randline(input_file)
                    except IOError as e:
                        parser.error("I/O error({0}): {1}".
                                     format(e.errno, e.strerror))
                    if int_c is -2:
                        int_c = generator.length_f()

                    if options.rep:
                        i = 0
                        try:
                            while i != int_c:
                                sys.stdout.write(generator.chooseline())
                                i += 1
                        except IndexError as err:
                            parser.error("Nothing to repeat")

                    else:
                        if int_c > generator.length_f():
                            int_c = generator.length_f()
                        sys.stdout.write(''.join(generator.choosenlines(int_c)))

        else:
            if len(args) != 0:
                parser.error("extra operand")
            if options.i_range.count('-') is not 1:
                parser.error("invalid input range: {0}".
                                 format(options.i_range))
            input_1, input_2 = options.i_range.split('-')
            try:
                LO = int(input_1)
            except ValueError as err:
                parser.error("invalid input range: {0}".
                                 format(options.i_range))
            try:
                HI = int(input_2)
            except ValueError as err:
                parser.error("invalid input range: {0}".
                                 format(options.i_range))
            if HI < LO:
                parser.error("invalid input range: {0}".
                                 format(options.i_range))
            iList = list(range(LO, HI+1))
            iList = [ str(i)+'\n' for i in iList ]
            max_count = HI+1-LO
            o_file =True


        if o_file:
            if int_c is -2:
                int_c = max_count
            if options.rep:
                i = 0
                try:
                     while i != int_c:
                         sys.stdout.write(random.choice(iList))
                         i += 1
                except IndexError as err:
                    parser.error("Nothing to repeat")
            else:
                if int_c > max_count:
                    int_c = max_count
                sys.stdout.write(''.join(random.sample(iList, int_c)))
    except KeyboardInterrupt:
        quit(0)



if __name__ == "__main__":
    main()
