; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [25 x i8] c"Testing all examples ...\00", align 1
@anon.string.1 = private unnamed_addr constant [10 x i8] c"Next line\00", align 1
@anon.string.2 = private unnamed_addr constant [16 x i8] c"Concluding line\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  tail call void @_p__void__void__println__string(ptr nonnull @anon.string.0) #1
  tail call void @_p__void__void__lineBreak() #1
  tail call void @_p__void__void__print__string(ptr nonnull @anon.string.1) #1
  tail call void @_p__void__void__lineBreak__int(i32 3) #1
  tail call void @_p__void__void__print__string(ptr nonnull @anon.string.2) #1
  ret i32 0
}

declare void @_p__void__void__println__string(ptr) local_unnamed_addr

declare void @_p__void__void__lineBreak() local_unnamed_addr

declare void @_p__void__void__print__string(ptr) local_unnamed_addr

declare void @_p__void__void__lineBreak__int(i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
