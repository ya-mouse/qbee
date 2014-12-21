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

class ProxyConsole(code.InteractiveConsole):
    """Proxy interactive console to remote interpreter."""

    def __init__(self, context):
        code.InteractiveConsole.__init__(self, context)
        self._completer = rlcompleter.Completer(None)


    def interact(self, banner=None):
        logging.info('Enter.')
        return code.InteractiveConsole.interact(self, banner)

    def complete(self, phrase, state):
        """Auto complete support for interactive console."""

        logging.debug('Enter, phrase=%r, state=%d.', phrase, state)

        # Allow tab key to simply insert spaces when proper.
        if phrase == '':
            if state == 0:
                return '    '
            return None

        return self._completer.complete(phrase, state)

    def runsource(self, source, filename="<input>", symbol="single"):
        logging.debug('Enter, source=%r.', source)

        return super().runsource(source, filename, symbol)

#        more, output = self._interpreter.runsource(source, filename)
#        if output:
#            self.write(output)

#        return more

def interact(banner=None, readfunc=None, context=None):
    """Start console and connect to remote console server."""

    console = ProxyConsole(context)

    if readfunc is not None:
        console.raw_input = readfunc
    else:
        try:
            import readline
            readline.set_completer(console.complete)
            readline.parse_and_bind('tab: complete')
        except ImportError:
            pass

    console.interact(banner)

fh = [
    logging.FileHandler('/dev/stderr', 'w')
]
fh[0].setLevel(logging.DEBUG)
logging.basicConfig(format='%(levelname)-8s [%(asctime)s] %(message)s', level=logging.DEBUG, handlers=fh)
