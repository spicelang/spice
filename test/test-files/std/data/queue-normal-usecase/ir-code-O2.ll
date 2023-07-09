; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Queue = type { ptr, i64, i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [24 x i8] c"Size: %d, Capacity: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %q1 = alloca %struct.Queue, align 8
  %1 = alloca i8, align 1
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  %5 = alloca i8, align 1
  %6 = alloca i8, align 1
  call void @_ZN5QueueIhE4ctorEv(ptr nonnull %q1) #2
  store i8 72, ptr %1, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %1) #2
  store i8 101, ptr %2, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %2) #2
  store i8 108, ptr %3, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %3) #2
  store i8 108, ptr %4, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %4) #2
  store i8 111, ptr %5, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %5) #2
  store i8 33, ptr %6, align 1
  call void @_ZN5QueueIhE4pushERKh(ptr nonnull %q1, ptr nonnull %6) #2
  %7 = call i64 @_ZN5QueueIhE7getSizeEv(ptr nonnull %q1) #2
  %8 = call i64 @_ZN5QueueIhE11getCapacityEv(ptr nonnull %q1) #2
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %7, i64 %8)
  %10 = call i1 @_ZN5QueueIhE7isEmptyEv(ptr nonnull %q1) #2
  br i1 %10, label %while.exit.L12, label %while.body.L12

while.body.L12:                                   ; preds = %0, %while.body.L12
  %11 = call ptr @_ZN5QueueIhE3popEv(ptr nonnull %q1) #2
  %12 = load i8, ptr %11, align 1
  %13 = sext i8 %12 to i32
  %putchar = call i32 @putchar(i32 %13)
  %14 = call i1 @_ZN5QueueIhE7isEmptyEv(ptr nonnull %q1) #2
  br i1 %14, label %while.exit.L12, label %while.body.L12

while.exit.L12:                                   ; preds = %while.body.L12, %0
  call void @_ZN5QueueIhE4dtorEv(ptr nonnull %q1) #2
  ret i32 0
}

declare void @_ZN5QueueIhE4ctorEv(ptr) local_unnamed_addr

declare void @_ZN5QueueIhE4pushERKh(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_ZN5QueueIhE7getSizeEv(ptr) local_unnamed_addr

declare i64 @_ZN5QueueIhE11getCapacityEv(ptr) local_unnamed_addr

declare i1 @_ZN5QueueIhE7isEmptyEv(ptr) local_unnamed_addr

declare ptr @_ZN5QueueIhE3popEv(ptr) local_unnamed_addr

declare void @_ZN5QueueIhE4dtorEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
