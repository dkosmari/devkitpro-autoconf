# Devkitpro Autoconf

This is a collection of Autoconf macros to make it easier to use the devkitPro toolchain.

The macros all use the [all-permissive
license](https://www.gnu.org/prep/maintain/html_node/License-Notices-for-Other-Files.html). They
depend on [Autoconf Archive](https://www.gnu.org/software/autoconf-archive/) macros, which
are usually installed alongside base Autoconf.


## Installation

0. `./bootstrap`

1. `./configure --prefix=/usr`

2. `make`

3. `make install`

Note the `--prefix=` option is needed to ensure the macros are installed in
`/usr/share/aclocal`. Check the output of `aclocal --print-ac-dir` to see it's pointing to
a non-standard directory, and modify the `--prefix=` option accordingly.


## Examples

Some usage examples are provided in the [examples](examples) directory.

Note that they use a [`./bootstrap`](common/bootstrap) script that just creates links to
the `.m4` macros in the [`macros`](macros) directory, to make sure the examples always use
the latest version.

When you use these macros for your own project, your `bootstrap` script should look like
this instead:

```
# Copy all non-standard macros to our local macro repository.
aclocal -I m4 --install
# Now invoke autoreconf to generate the configure script.
exec autoreconf --install "$@"
```

You should also add `m4` to the source control.
