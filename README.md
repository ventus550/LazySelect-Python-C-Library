# LazySelect-Python-C-Library
Python library implementing LazySelect algorithm, which given a list of distinct integers finds the k-th smallest element.

### Installing module
```
sudo python3 setup.py install
```

Then simply import the module to the python file and run it as follows:
```python
from LazySelect import select
print( select([2, 3, 1, 4, 13, 8], 3) )
```

This will print out the 3rd smallest element in the list.
