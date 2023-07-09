; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asDouble1 == 1.0' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asDouble2 == 0.0' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'asInt1 == 1' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'asInt2 == 0' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'asShort1 == 1s' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'asShort2 == 0s' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong1 == 1l' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong2 == 0l' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'asByte1 == (byte) 1' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'asByte2 == (byte) 0' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_Z8toDoubleb(i1 true) #2
  %2 = fcmp oeq double %1, 1.000000e+00
  br i1 %2, label %assert.exit.L6, label %assert.then.L6, !prof !0

assert.then.L6:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L6:                                   ; preds = %0
  %4 = tail call double @_Z8toDoubleb(i1 false) #2
  %5 = fcmp oeq double %4, 0.000000e+00
  br i1 %5, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %assert.exit.L6
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L8:                                   ; preds = %assert.exit.L6
  %7 = tail call i32 @_Z5toIntb(i1 true) #2
  %8 = icmp eq i32 %7, 1
  br i1 %8, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L8
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L8
  %10 = tail call i32 @_Z5toIntb(i1 false) #2
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L12
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L12
  %13 = tail call i16 @_Z7toShortb(i1 true) #2
  %14 = icmp eq i16 %13, 1
  br i1 %14, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L14
  %15 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L14
  %16 = tail call i16 @_Z7toShortb(i1 false) #2
  %17 = icmp eq i16 %16, 0
  br i1 %17, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L18
  %18 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L18
  %19 = tail call i64 @_Z6toLongb(i1 true) #2
  %20 = icmp eq i64 %19, 1
  br i1 %20, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L20
  %21 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L20
  %22 = tail call i64 @_Z6toLongb(i1 false) #2
  %23 = icmp eq i64 %22, 0
  br i1 %23, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L24
  %24 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L24
  %25 = tail call i8 @_Z6toByteb(i1 true) #2
  %26 = icmp eq i8 %25, 1
  br i1 %26, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L26
  %27 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L26
  %28 = tail call i8 @_Z6toByteb(i1 false) #2
  %29 = icmp eq i8 %28, 0
  br i1 %29, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L30
  %30 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L30
  %31 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_Z8toDoubleb(i1) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare i32 @_Z5toIntb(i1) local_unnamed_addr

declare i16 @_Z7toShortb(i1) local_unnamed_addr

declare i64 @_Z6toLongb(i1) local_unnamed_addr

declare i8 @_Z6toByteb(i1) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
