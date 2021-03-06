# Zenom Core

This is Zenom Core program which is capable of running real time programs 
at the command line. It is stripped down version of [Zenom](https://github.com/GTUKontrolRobotik/Zenom). 

## How to Use Zenom Core

* First create your real time task using `Zenom`. 
* Implement `doloop` method of your `ControlBase` class
* Add `LogVariable`, `ControlVariable`
* `LogVariable`'s are used to watch your tasks outputs
* `ControlVariable`'s are used as input to your tasks
* Set `frequency` and `duration` of your task in `Zenom` main window
* Save your project
* Now here comes to `ZenomCore` part:
  * Open your project by typing on your favorite terminal `zenomcore ~/path/to/project.znm`
  * `ZenomCore` automatically will run your realtime task, and save the log variables to `test.m` on your project folder.
  * You can now open `Matlab` or `Octave` to see the results

![Commandline Output of ZenomCore][cmd]

## Visulize the Log Variables

`ZenomCore` will save the log variables to your project folder after execution is done.
You can then open the `test.m` on either `Matlab` or `Octave`. Run this script and
using the code snippet below you can plot the `sine` log variable.

```
figure(1);
plot(sine_t, sine(:,1));
l = xlabel("time[sec]");
set(l,'FontSize',14);
l = ylabel('sine(t)');
set(l,'FontSize',14);
set(gcf, 'Position',  [100, 100, 750, 500])
grid on;
grid minor;
```

![Octave Plotting][octave]

As you can see, the result is same for both `Zenom` and `ZenomCore` as expected.

![Zenom Plotting][zenom]

## Cross Compiling

You can cross compile `ZenomCore` to any Qt4 supporting linux environment.
Here is the list of needed shared libraries:

![Shared Libraries][shared]

`libznm-core` and `libznm-tools` are supplied within this project.

Using `QtCreator` you can add new Qt Kits. Create a Qt Kit for your platform and
recompile the project with it.

## Credits

* First version of `Zenom` with `Xenomai` is created by:
  * Cüneyt Ay
  * Hüsnü Karaküçük

* Modifications done by:
  * [Ahmet Soyyiğit][ahmet]
  * [Mesih Veysi Kılınç][mesih]

* `ZenomCore` is derived from `Zenom` by:
  * [Mesih Veysi Kılınç][mesih]

Made in [RobotLab][robotlab] @ [Gebze Technical University][gtu]

[octave]: https://github.com/GTUKontrolRobotik/ZenomCore/blob/master/doc/octave.png
[cmd]: https://github.com/GTUKontrolRobotik/ZenomCore/blob/master/doc/cmd.png
[zenom]: https://github.com/GTUKontrolRobotik/ZenomCore/blob/master/doc/zenom.png
[shared]: https://github.com/GTUKontrolRobotik/ZenomCore/blob/master/doc/shared.png
[gtu]: https://www.gtu.edu.tr/
[robotlab]: https://bilmuh.gtu.edu.tr/robotlab/contact.html
[mesih]: https://github.com/MesihK
[ahmet]: https://github.com/ahmedius2
