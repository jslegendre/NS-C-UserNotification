#include <CoreFoundation/CoreFoundation.h>
#include <objc/objc-runtime.h>

int main(int argc, char** argv) {
    id pool = (id)objc_getClass("NSAutoreleasePool");
    
    pool = objc_msgSend(pool,
                        sel_registerName("alloc"),
                        sel_registerName("init"));
    
    Class bundle = objc_getClass("NSBundle");

    //Block swizzle 
    class_replaceMethod(bundle,
                        sel_registerName("bundleIdentifier"),
                        method_getImplementation((Method)^{return CFSTR("com.apple.finder");}),
                        NULL);
    
    id notifCenter = objc_msgSend((id)objc_getClass("NSUserNotificationCenter"),
                                  sel_registerName("defaultUserNotificationCenter"));
    
    id notif = objc_msgSend((id)objc_getClass("NSUserNotification"),
                            sel_registerName("alloc"),
                            sel_registerName("init"));
    
    objc_msgSend(notif, sel_registerName("setTitle:"), CFSTR("Title"));
    //objc_msgSend(notif, sel_registerName("setSubtitle:"), CFSTR("Subtitle"));
    objc_msgSend(notif, sel_registerName("setInformativeText:"), CFSTR("Informative text"));
    
    objc_msgSend(notifCenter, sel_registerName("deliverNotification:"), notif);
    
    sleep(1);
    
    objc_msgSend(pool, sel_registerName("release"));

    return 0;
}