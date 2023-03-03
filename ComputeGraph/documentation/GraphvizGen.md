

# Graphviz generation

## API

```python
def graphviz(self,f,config=Configuration())
```

It is a method of the `Schedule` object returned by `computeSchedule`.

* `f` : Opened file where to write the graphviz description
* `config` : An optional configuration object

## Options for the graphviz generator

### horizontal (default = True)

Horizontal or vertical layout for the graph.

### displayFIFOBuf (default = False)

By default, the graph is displaying the FIFO sizes computed as result of the scheduling. If you want to know the FIFO variable names used in the code, you can set this option to true and the graph will display the FIFO variable names.