"""
Context file for testing. Make sure to import needed submodules.
"""
import sys
import os
from contextlib import contextmanager
from io import StringIO
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src')))


# pylint: disable=unused-import,wrong-import-position, import-error
import openroute
import opencaching
import utils.matrix as matrix_utils


@contextmanager
def captured_output():
    """
    Context manager for redirecting stdout and stderr.
    Usage: `with captured_output() as (out, err):`
    """
    new_out, new_err = StringIO(), StringIO()
    old_out, old_err = sys.stdout, sys.stderr
    try:
        sys.stdout, sys.stderr = new_out, new_err
        yield sys.stdout, sys.stderr
    finally:
        sys.stdout, sys.stderr = old_out, old_err
