# opengl errors

## 使用addr2line 解析地址对应的符号

        [OpenGL Error (1280)
        Depth: 0, func name: ./demo(+0x7f1b) [0x55e721157f1b]
        Depth: 1, func name: ./demo(+0x8a3b) [0x55e721158a3b]
        Depth: 2, func name: /lib/x86_64-linux-gnu/libc.so.6(+0x29d90) [0x7f33baf5bd90]
        Depth: 3, func name: /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0x80) [0x7f33baf5be40]
        Depth: 4, func name: ./demo(+0x7e25) [0x55e721157e25]
        ^C
        wangli@wangli-System-Product-Name:~/project/linux_learn_diary/opengl_study/6$ addr2line -a 0x8a3b -f -e demo
        0x0000000000008a3b
        main
        /home/wangli/project/linux_learn_diary/opengl_study/6/demo.cpp:190 (discriminator 5)