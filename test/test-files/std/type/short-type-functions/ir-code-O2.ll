; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'asDouble == 123.0' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asInt == -345' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong == 45l' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'asByte == (byte) 12' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asChar == 'B'' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asBool1 == true' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asBool2 == false' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'isPowerOfTwo(16s)' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition '!isPowerOfTwo(31s)' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_f__void__double__toDouble__short(i16 123) #2
  %2 = fcmp oeq double %1, 1.230000e+02
  br i1 %2, label %assert.exit.L6, label %assert.then.L6

assert.then.L6:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L6:                                   ; preds = %0
  %4 = tail call i32 @_f__void__int__toInt__short(i16 -345) #2
  %5 = icmp eq i32 %4, -345
  br i1 %5, label %assert.exit.L10, label %assert.then.L10

assert.then.L10:                                  ; preds = %assert.exit.L6
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L6
  %7 = tail call i64 @_f__void__long__toLong__short(i16 45) #2
  %8 = icmp eq i64 %7, 45
  br i1 %8, label %assert.exit.L14, label %assert.then.L14

assert.then.L14:                                  ; preds = %assert.exit.L10
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %10 = tail call i8 @_f__void__byte__toByte__short(i16 12) #2
  %11 = icmp eq i8 %10, 12
  br i1 %11, label %assert.exit.L18, label %assert.then.L18

assert.then.L18:                                  ; preds = %assert.exit.L14
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L14
  %13 = tail call i8 @_f__void__char__toChar__short(i16 66) #2
  %14 = icmp eq i8 %13, 66
  br i1 %14, label %assert.exit.L22, label %assert.then.L22

assert.then.L22:                                  ; preds = %assert.exit.L18
  %15 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L18
  %16 = tail call i1 @_f__void__bool__toBool__short(i16 1) #2
  br i1 %16, label %assert.exit.L31, label %assert.then.L31

assert.then.L31:                                  ; preds = %assert.exit.L22
  %17 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L22
  %18 = tail call i1 @_f__void__bool__toBool__short(i16 0) #2
  br i1 %18, label %assert.then.L33, label %assert.exit.L33

assert.then.L33:                                  ; preds = %assert.exit.L31
  %19 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L31
  %20 = tail call i1 @_f__void__bool__isPowerOfTwo__short(i16 16) #2
  br i1 %20, label %assert.exit.L36, label %assert.then.L36

assert.then.L36:                                  ; preds = %assert.exit.L33
  %21 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L33
  %22 = tail call i1 @_f__void__bool__isPowerOfTwo__short(i16 31) #2
  br i1 %22, label %assert.then.L37, label %assert.exit.L37

assert.then.L37:                                  ; preds = %assert.exit.L36
  %23 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_f__void__double__toDouble__short(i16) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare i32 @_f__void__int__toInt__short(i16) local_unnamed_addr

declare i64 @_f__void__long__toLong__short(i16) local_unnamed_addr

declare i8 @_f__void__byte__toByte__short(i16) local_unnamed_addr

declare i8 @_f__void__char__toChar__short(i16) local_unnamed_addr

declare i1 @_f__void__bool__toBool__short(i16) local_unnamed_addr

declare i1 @_f__void__bool__isPowerOfTwo__short(i16) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
