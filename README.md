stuart
======
stuart is a Stress Tester for UARTs that runs on STM32F103xx boards.

The contents of [`src/payload.txt`](src/payload.txt) are sent repeatedly until a fixed number of bytes have been sent.

The default configuration sends 1MiB at 115200 baud.

Configuration
=============
The firmware can be built with a different baudrate or test size by overriding environment variables during the build process:

    touch stuart.c && make BAUDRATE=230400
    make flash

You can also add overrides to `local.mk`, which is automatically included in the main Makefile if present.

Here's a sample `local.mk`:

    # Transmit 230400 bits per second
    BAUDRATE=230400
    # Send 2MiB
    NUM_TEST_BYTES=2097152

