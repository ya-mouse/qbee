#
# cmdline routines
#
# Copyright (c) 2014 Anton D. Kachalov <mouse@yandex.ru>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
import rlcompleter
import logging
import code
import builtins
import __main__
import qb

class Completer:
    def __init__(self, namespace = None):
        """Create a new completer for the command line.

        Completer([namespace]) -> completer instance.

        If unspecified, the default namespace where completions are performed
        is __main__ (technically, __main__.__dict__). Namespaces should be
        given as dictionaries.

        Completer instances should be used as the completion mechanism of
        readline via the set_completer() call:

        readline.set_completer(Completer(my_namespace).complete)
        """

        if namespace and not isinstance(namespace, dict):
            raise TypeError('namespace must be a dictionary')

        # Don't bind to namespace quite yet, but flag whether the user wants a
        # specific namespace or to use __main__.__dict__. This will allow us
        # to bind to __main__.__dict__ at completion time, not now.
        if namespace is None:
            self.use_main_ns = 1
        else:
            self.use_main_ns = 0
            self.namespace = namespace

    def complete(self, text, state):
        """Return the next possible completion for 'text'.

        This is called successively with state == 0, 1, 2, ... until it
        returns None.  The completion should begin with 'text'.

        """
        if self.use_main_ns:
            self.namespace = __main__.__dict__

        if state == 0:
            if "." in text:
                self.matches = self.attr_matches(text)
            else:
                self.matches = self.global_matches(text)
        try:
            return self.matches[state]
        except IndexError:
            return None

    def _callable_postfix(self, val, word):
        try:
            if callable(val):
                word = word + "("
        except TypeError:
            # Just skip RPC types
            pass
        return word

    def global_matches(self, text):
        """Compute matches when text is a simple name.

        Return a list of all keywords, built-in functions and names currently
        defined in self.namespace that match.

        """
        import keyword
        matches = []
        n = len(text)
        for word in keyword.kwlist:
            if word[:n] == text:
                matches.append(word)
        for nspace in [builtins.__dict__, self.namespace]:
            for word, val in nspace.items():
                if word[:n] == text and word != "__builtins__":
                    matches.append(self._callable_postfix(val, word))
        return matches

    def attr_matches(self, text):
        """Compute matches when text contains a dot.

        Assuming the text is of the form NAME.NAME....[NAME], and is
        evaluable in self.namespace, it will be evaluated and its attributes
        (as revealed by dir()) are used as possible completions.  (For class
        instances, class members are also considered.)

        WARNING: this can still invoke arbitrary C code, if an object
        with a __getattr__ hook is evaluated.

        """
        import re
        m = re.match(r"(\w+(\.\w+)*)\.(\w*)", text)
        if not m:
            return []
        expr, attr = m.group(1, 3)
        try:
            thisobject = eval(expr, self.namespace)
        except Exception as e:
            return []

        # get the content of the object, except __builtins__
        words = dir(thisobject)
        if "__builtins__" in words:
            words.remove("__builtins__")

#        if hasattr(thisobject, '__class__'):
#            words.append('__class__')
#            words.extend(get_class_members(thisobject.__class__))
        matches = []
        n = len(attr)
        for word in words:
            if word[:n] == attr and hasattr(thisobject, word) and word[0] != '_':
                val = getattr(thisobject, word)
                # Skip nested class definitions
                if isinstance(val, type):
                    continue
                word = self._callable_postfix(val, "%s.%s" % (expr, word))
                matches.append(word)
        return matches

def get_class_members(klass):
    ret = dir(klass)
    if hasattr(klass,'__bases__'):
        for base in klass.__bases__:
            ret = ret + get_class_members(base)
    return ret

class qb_console(code.InteractiveConsole):
    """Proxy interactive console to remote interpreter."""

    def __init__(self, context):
        code.InteractiveConsole.__init__(self, context)
        self._completer = Completer(None) #context)

    def interact(self, banner=None):
#        logging.info('Enter.')
        return code.InteractiveConsole.interact(self, banner)

    def complete(self, phrase, state):
        """Auto complete support for interactive console."""

#        logging.debug('Enter, phrase=%r, state=%d.', phrase, state)

        # Allow tab key to simply insert spaces when proper.
        if phrase == '':
            if state == 0:
                return ''
            return None

        return self._completer.complete(phrase, state)

    def runsource(self, source, filename="<input>", symbol="single"):
#        logging.debug('Enter, source=%r.', source)

        return super().runsource(source, filename, symbol)

#        more, output = self._interpreter.runsource(source, filename)
#        if output:
#            self.write(output)

#        return more

def interact(banner=None, readfunc=None, context=None):
    """Start console and connect to remote console server."""

    console = qb_console(context)

    if readfunc is not None:
        console.raw_input = readfunc
    else:
        try:
            import atexit, os
            import readline
            readline.set_completer(console.complete)
            readline.parse_and_bind('tab: complete')
            histfile = os.path.join(os.path.expanduser("~"), ".qbee_history")
            try:
                readline.read_history_file(histfile)
            except FileNotFoundError:
                pass
            atexit.register(readline.write_history_file, histfile)
        except ImportError:
            pass

    console.interact(banner)

fh = [
    logging.FileHandler('/dev/stderr', 'w')
]
fh[0].setLevel(logging.DEBUG)
logging.basicConfig(format='%(levelname)-8s [%(asctime)s] %(message)s', level=logging.DEBUG, handlers=fh)
