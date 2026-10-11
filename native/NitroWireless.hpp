#pragma once
// Host side of the DS wireless manager (WM): a radio with no other console in
// range. Requests complete asynchronously, like the ARM7 replies they replace.
namespace pc {
// Delivers the callbacks that are due; called once per frame.
void advanceWireless();
}
