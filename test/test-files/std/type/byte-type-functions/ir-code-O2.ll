; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asDouble == 15.0' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 'asInt == 9' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asShort == 6s' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong == 63l' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asBool1 == true' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asBool2 == false' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_f__void__double__toDouble__byte(i8 15) #2
  %2 = fcmp oeq double %1, 1.500000e+01
  br i1 %2, label %assert.exit.L6, label %assert.then.L6

assert.then.L6:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.0)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L6:                                   ; preds = %0
  %4 = tail call i32 @_f__void__int__toInt__byte(i8 9) #2
  %5 = icmp eq i32 %4, 9
  br i1 %5, label %assert.exit.L10, label %assert.then.L10

assert.then.L10:                                  ; preds = %assert.exit.L6
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.1)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L6
  %7 = tail call i16 @_f__void__short__toShort__byte(i8 6) #2
  %8 = icmp eq i16 %7, 6
  br i1 %8, label %assert.exit.L14, label %assert.then.L14

assert.then.L14:                                  ; preds = %assert.exit.L10
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.2)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %10 = tail call i64 @_f__void__long__toLong__byte(i8 63) #2
  %11 = icmp eq i64 %10, 63
  br i1 %11, label %assert.exit.L18, label %assert.then.L18

assert.then.L18:                                  ; preds = %assert.exit.L14
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.3)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L14
  %13 = tail call i1 @_f__void__bool__toBool__byte(i8 1) #2
  br i1 %13, label %assert.exit.L26, label %assert.then.L26

assert.then.L26:                                  ; preds = %assert.exit.L18
  %14 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.4)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L18
  %15 = tail call i1 @_f__void__bool__toBool__byte(i8 0) #2
  br i1 %15, label %assert.then.L28, label %assert.exit.L28

assert.then.L28:                                  ; preds = %assert.exit.L26
  %16 = tail call i32 (ptr, ...) @printf(ptr nonnull @anon.string.5)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  %17 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0)
  ret i32 0
}

declare double @_f__void__double__toDouble__byte(i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare i32 @_f__void__int__toInt__byte(i8) local_unnamed_addr

declare i16 @_f__void__short__toShort__byte(i8) local_unnamed_addr

declare i64 @_f__void__long__toLong__byte(i8) local_unnamed_addr

declare i1 @_f__void__bool__toBool__byte(i8) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
