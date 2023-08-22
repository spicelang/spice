; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'asDouble == 123.0' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asInt == -345' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong == 45l' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'asByte == (byte) 12' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asChar == 'B'' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'isPowerOfTwo(16s)' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition '!isPowerOfTwo(31s)' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_Z8toDoubles(i16 123) #3
  %2 = fcmp oeq double %1, 1.230000e+02
  br i1 %2, label %assert.exit.L6, label %assert.then.L6, !prof !0

assert.then.L6:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %0
  %4 = tail call i32 @_Z5toInts(i16 -345) #3
  %5 = icmp eq i32 %4, -345
  br i1 %5, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L6
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L6
  %7 = tail call i64 @_Z6toLongs(i16 45) #3
  %8 = icmp eq i64 %7, 45
  br i1 %8, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L10
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %10 = tail call i8 @_Z6toBytes(i16 12) #3
  %11 = icmp eq i8 %10, 12
  br i1 %11, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L14
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L14
  %13 = tail call i8 @_Z6toChars(i16 66) #3
  %14 = icmp eq i8 %13, 66
  br i1 %14, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L18
  %15 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L18
  %16 = tail call i1 @_Z6toBools(i16 1) #3
  %17 = tail call i1 @_Z6toBools(i16 0) #3
  %18 = tail call i1 @_Z12isPowerOfTwos(i16 16) #3
  br i1 %18, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L22
  %19 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L22
  %20 = tail call i1 @_Z12isPowerOfTwos(i16 31) #3
  br i1 %20, label %assert.then.L37, label %assert.exit.L37, !prof !1

assert.then.L37:                                  ; preds = %assert.exit.L36
  %21 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_Z8toDoubles(i16) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare i32 @_Z5toInts(i16) local_unnamed_addr

declare i64 @_Z6toLongs(i16) local_unnamed_addr

declare i8 @_Z6toBytes(i16) local_unnamed_addr

declare i8 @_Z6toChars(i16) local_unnamed_addr

declare i1 @_Z6toBools(i16) local_unnamed_addr

declare i1 @_Z12isPowerOfTwos(i16) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
