# automake学习

## 资源
[官方手册](https://www.gnu.org/software/automake/manual/automake.html)

### 介绍

Automake is a tool for automatically generating Makefile.ins from files called Makefile.am. 

    Automake是一个用来将Makefile.in文件生成Makefile.am文件的工具。

Each Makefile.am is basically a series of make variable definitions, with rules being thrown in occasionally. 

    每个Makefile.am基本上都是一系列make变量定义，偶尔会抛出一些规则。 ---

The generated Makefile.ins are compliant with the GNU Makefile standards.

    生成的Makefile.ins符合GNU Makefile标准。

The GNU Makefile Standards Document (see [Makefile Conventions](https://www.gnu.org/prep/standards/standards.html#Makefile-Conventions) in The GNU Coding Standards) is long, complicated, and subject to change.

    GNU Makefile标准文档又长又复杂，还可能会更改。

The goal of Automake is to remove the burden of Makefile maintenance from the back of the individual GNU maintainer (and put it on the back of the Automake maintainers).

    Automake的目标是将Makefile维护的负担从单个GNU维护人员的背后移除(并将其放在Automake维护人员的背后)。

The typical Automake input file is simply a series of variable definitions. 

    典型的Automake输入文件只是一系列变量定义。

Each such file is processed to create a Makefile.in.

    每个文件被用来生成一个Makefile.in文件。

Automake does constrain a project in certain ways; for instance, it assumes that the project uses Autoconf (see Introduction in The Autoconf Manual), and enforces certain restrictions on the configure.ac contents.