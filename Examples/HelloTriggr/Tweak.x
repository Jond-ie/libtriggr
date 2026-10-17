// libtriggr test: one trigger (fired from a terminal) and one action (writes a log line). Dev only.
#import <Triggr.h>
#include <notify.h>

static void DemoLog(NSString *s) {
    FILE *f = fopen("/var/tmp/hellotriggr.log", "a");
    if (f) { fprintf(f, "%.3f %s\n", CFAbsoluteTimeGetCurrent(), s.UTF8String); fclose(f); }
}

%ctor {
    TGTriggerInfo *t = [TGTriggerInfo new];
    t.identifier = @"com.example.hellotriggr.ping";
    t.title = @"Demo Ping";
    t.group = @"Hello Triggr";
    t.footer = @"Sent from a terminal with notifyutil.";
    [[TGTriggr shared] registerTrigger:t];

    TGActionInfo *a = [TGActionInfo new];
    a.identifier = @"com.example.hellotriggr.hello";
    a.title = @"Demo Hello";
    a.group = @"Hello Triggr";
    a.symbolName = @"hand.wave.fill";
    [[TGTriggr shared] registerAction:a handler:^(TGActionContext *ctx) {
        DemoLog([NSString stringWithFormat:@"hello ran (trigger=%@ app=%@ locked=%d)", ctx.triggerIdentifier, ctx.frontmostApp, ctx.locked]);
    }];

    int token;
    notify_register_dispatch("com.example.hellotriggr/ping", &token, dispatch_get_main_queue(), ^(int x) {
        BOOL handled = [[TGTriggr shared] sendTrigger:@"com.example.hellotriggr.ping"];
        DemoLog([NSString stringWithFormat:@"sendTrigger -> %d", handled]);
    });
    DemoLog(@"demo loaded");
}
