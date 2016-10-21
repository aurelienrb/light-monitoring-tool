# Light monitoring tool

> Server monitoring and performance tracking kept simple and easy!

A simple standalone C++ program (no dependency) to be run on a machine in order to track and record CPU/RAM/Disk usage. It is intended to be a simple and cheap alternative to more sofisticated software sush as [Monit](https://mmonit.com/monit/) or [Nagios](https://www.nagios.org/).

## Principe

The tool can serve perf data over HTTP via a REST API (json) and HTML/Javascript files to display them dynamically on a browser.

It can also generate a report to be send by email via a 3rd party software such as sendmail.

## Design principles

- **low CPU/RAM requirements**: should run unnoticed on embeded devices such as the first Raspberry Pi (single-core ARM, 512 Mo of RAM)
- **easy to install and manage**: just a standalone executable (console mode) that can be fully configured via command line
- **keep it simple**: collect and summarize basic performance metrics (CPU/Ram/Disk) that are useful for software developers or hobbyists. Don't try to compete with advanced solutions used by administrators.   
