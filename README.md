# libtriggr

Let your tweak plug into **Triggr**: add your own triggers, add your own actions, and put a
**"Configure via Triggr"** button in your settings. Everything runs on the device.

If you remember Activator's `libactivator`, this is the same idea for modern rootless jailbreaks.

- **Triggers**: your tweak says "this happened" (a double tap on your view, your tweak's event). Users pick what
  it runs in Triggr, from any of Triggr's actions or other tweaks' actions.
- **Actions**: your code, in Triggr's action list. Users assign it to any trigger: a button press, a gesture,
  a charger plugged in, someone else's trigger…
- **Configure via Triggr**: push Triggr's own picker from your settings, for one of your triggers.

Requires Triggr 2.0 or newer (iOS 15+, rootless).

## Setup

1. Add the header: copy [`Triggr.h`](Triggr.h) into your project (or use `/var/jb/usr/include/triggr/Triggr.h`
   from a device with Triggr installed).
2. Link the library. Copy `libtriggr.dylib` from a device (`/var/jb/usr/lib/libtriggr.dylib`) into your
   project, e.g. `lib/`, then in your Makefile:
   ```make
   MyTweak_LDFLAGS = -Llib -ltriggr -rpath /var/jb/usr/lib
   ```
3. Depend on Triggr in your `control` file:
   ```
   Depends: com.johndie.triggr (>= 2.0)
   ```
   Want Triggr to be optional instead? Don't link; `dlopen("/var/jb/usr/lib/libtriggr.dylib", RTLD_NOW)` and
   use `NSClassFromString(@"TGTriggr")`. Always check `TGTriggrAvailable()` first.

## Add a trigger

Register it once, from your **SpringBoard** tweak (most tweaks have one), then send it when it happens:

```objc
#import <Triggr.h>

%ctor {
    TGTriggerInfo *t = [TGTriggerInfo new];
    t.identifier = @"com.you.mytweak.dockdoubletap";   // unique, reverse-DNS, no spaces or "/"
    t.title = @"Double Tap Dock";
    t.group = @"MyTweak";                              // shown as its own group in Triggr
    t.footer = @"Two quick taps on an empty part of the Dock.";
    [[TGTriggr shared] registerTrigger:t];
}

// Later, when it happens:
if ([[TGTriggr shared] sendTrigger:@"com.you.mytweak.dockdoubletap"]) return; // assigned: skip your default
// not assigned: carry on with your tweak's normal behaviour
```

- `sendTrigger:` returns **YES** when something is assigned (and runs it), so you can skip your own default,
  like Activator's "handled" events. It returns **NO** when nothing is assigned.
- It works from **any process** (an app, a daemon, SpringBoard). Outside SpringBoard it costs almost nothing
  when unassigned: nothing is sent at all.
- Expensive detection? Check `isTriggerAssigned:` first and skip the work when nobody uses it.

## Add an action

```objc
TGActionInfo *a = [TGActionInfo new];
a.identifier = @"com.you.mytweak.toggle";
a.title = @"Toggle MyTweak";
a.group = @"MyTweak";            // its own category in Triggr's action picker
a.symbolName = @"bolt.fill";     // SF Symbol for the category
[[TGTriggr shared] registerAction:a handler:^(TGActionContext *context) {
    // runs on the main thread, inside your tweak (no shell, no delay)
    // context.triggerIdentifier, context.frontmostApp (nil on the Home / Lock Screen), context.locked
    MyTweakToggle();
}];
```

- Register actions from your **SpringBoard** tweak: actions run in the process that registered them, and
  Triggr runs assignments in SpringBoard.
- If your tweak is removed, its actions show as "Missing: …" in Triggr and are skipped.

## "Configure via Triggr" in your settings

In your PreferenceLoader bundle:

```objc
- (void)configureTriggr {
    UIViewController *picker = [TGTriggr pickerForTrigger:@"com.you.mytweak.dockdoubletap"];
    if (picker) [self.navigationController pushViewController:picker animated:YES];
}
```

It's Triggr's own picker: every action, Only If conditions and a Test button. Choices save straight into
Triggr (Anywhere). Disable the row when `TGTriggrAvailable()` is NO.

## Good to know

- **Zero cost when unused:** nothing listens for your trigger until someone assigns it. Registering is just a
  dictionary entry.
- **Identifiers are forever:** users' assignments point at them. Don't rename them between versions.
- **Register early and every launch** (in `%ctor`). Registering the same identifier again updates its title.
- `places` on the info objects is reserved for a later version and not used yet.
- Example tweak: [`Examples/HelloTriggr`](Examples/HelloTriggr) (one trigger, one action, a settings page with
  Configure via Triggr).

## Questions and requests

Open an issue on the libtriggr repo. Triggr itself is closed source; libtriggr (this header, docs and examples)
is free to use in any tweak, free or paid.
