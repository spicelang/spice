; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [25 x i8] c"Testing all examples ...\00", align 1
@anon.string.1 = private unnamed_addr constant [10 x i8] c"Next line\00", align 1
@anon.string.2 = private unnamed_addr constant [16 x i8] c"Concluding line\00", align 1
@anon.string.4 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.7 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  tail call void @_Z7printlnPc(ptr nonnull @anon.string.0) #1
  tail call void @_Z9lineBreakv() #1
  tail call void @_Z5printPc(ptr nonnull @anon.string.1) #1
  tail call void @_Z9lineBreakj(i32 3) #1
  tail call void @_Z7printlnPc(ptr nonnull @anon.string.2) #1
  tail call void @_Z15printFixedWidthPcj(ptr nonnull @anon.string.4, i32 10) #1
  tail call void @_Z9lineBreakv() #1
  tail call void @_Z15printFixedWidthPcjb(ptr nonnull @anon.string.4, i32 10, i1 true) #1
  tail call void @_Z9lineBreakv() #1
  tail call void @_Z15printFixedWidthPcj(ptr nonnull @anon.string.7, i32 12) #1
  tail call void @_Z9lineBreakv() #1
  tail call void @_Z15printFixedWidthPcjb(ptr nonnull @anon.string.7, i32 7, i1 false) #1
  tail call void @_Z9lineBreakv() #1
  tail call void @_Z15printFixedWidthPcjb(ptr nonnull @anon.string.7, i32 5, i1 true) #1
  ret i32 0
}

declare void @_Z7printlnPc(ptr) local_unnamed_addr

declare void @_Z9lineBreakv() local_unnamed_addr

declare void @_Z5printPc(ptr) local_unnamed_addr

declare void @_Z9lineBreakj(i32) local_unnamed_addr

declare void @_Z15printFixedWidthPcj(ptr, i32) local_unnamed_addr

declare void @_Z15printFixedWidthPcjb(ptr, i32, i1) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
