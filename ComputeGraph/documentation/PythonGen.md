# Python code generation

## API

```python
def pythoncode(self,directory,config=Configuration())
```

It is a method of the `Schedule` object returned by `computeSchedule`.

It generate Python code to implement the static schedule.

* `directory` : The directory where to generate the C++ files
* `config` : An optional configuration object

## Options for Python code generation

### pyOptionalArgs (default = "")

Optional arguments to pass to the Python version of the scheduler function

### customPythonName (default = "custom")

Name of custom header in generated Python code. If you use several scheduler, you may want to use different headers for each one.

### appNodesPythonName (default = "appnodes")

Name of AppNodes header in generated Python code. If you use several scheduler, you may want to use different headers for each one.

### schedulerPythonFileName (default = "sched")

Name of scheduler file in generated Python code. If you use several scheduler, you may want to use different headers for each one.

If the option is set to `xxx`, the name generated will be `xxx.py`