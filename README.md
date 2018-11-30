GUIWorker Cross Thread Call Example
===================================

This is an example of the complications of cross thread calling for
C++ member functions. It is possible and quite easy but requires a
large quantity of boilerplate code to invoke the function.

To call GUIWorkerForm::ExecuteTaskAWt with a single argument requires
10 lines of setup code and 2 lines of finalisation code. It also
requires two methods in the class where ideally I would prefer one.

C# makes use of Delegates to resolve this issue, it is not ideal but
better than the situation in this project.

According to the C++11 standard I should be able to call a member
function in a *new* thread like this:

    std::thread mythread = std::thread(&GUIWorkerForm::ExecuteTaskBWt, this, <arg>);

This should create a new thread, and call the `&GUIWorkerForm::ExecuteTaskBWt`
method with `this` and the proivded argument that is fine but I want to
use an *existing* thread (see workerthread.c).

Ideally, from the GUI thread I want to call a method like this where
`wt_ctx` is the context of the worker thread created earlier:

    CallMethodFromWorker(wt_ctx, &GUIWorkerForm::ExecuteTaskBWt, this, <arg>);

This requires the use of template functions and variadic arguments...

