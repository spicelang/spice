; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [59 x i8] c"Assertion failed: Condition 'i == 127' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %i, align 4
  %1 = load i32, ptr %i, align 4
  %2 = add nsw i32 %1, 2
  store i32 %2, ptr %i, align 4
  %3 = load i32, ptr %i, align 4
  %4 = mul nsw i32 %3, 2
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = shl i32 %5, 2
  store i32 %6, ptr %i, align 4
  %7 = load i32, ptr %i, align 4
  %8 = ashr i32 %7, 2
  store i32 %8, ptr %i, align 4
  %9 = load i32, ptr %i, align 4
  %10 = sdiv i32 %9, 2
  store i32 %10, ptr %i, align 4
  %11 = load i32, ptr %i, align 4
  %12 = sub nsw i32 %11, 2
  store i32 %12, ptr %i, align 4
  %13 = load i32, ptr %i, align 4
  %14 = icmp eq i32 %13, 1
  br i1 %14, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %0
  %16 = load i32, ptr %i, align 4
  %17 = add nsw i32 %16, 494665727
  store i32 %17, ptr %i, align 4
  %18 = load i32, ptr %i, align 4
  %19 = sdiv i32 %18, 2
  store i32 %19, ptr %i, align 4
  %20 = load i32, ptr %i, align 4
  %21 = mul nsw i32 %20, 2
  store i32 %21, ptr %i, align 4
  %22 = load i32, ptr %i, align 4
  %23 = sub nsw i32 %22, 494665727
  store i32 %23, ptr %i, align 4
  %24 = load i32, ptr %i, align 4
  %25 = icmp eq i32 %24, 1
  br i1 %25, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L9
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L9
  store i32 123, ptr %i, align 4
  %27 = load i32, ptr %i, align 4
  %28 = srem i32 %27, 2
  store i32 %28, ptr %i, align 4
  %29 = load i32, ptr %i, align 4
  %30 = icmp eq i32 %29, 1
  br i1 %30, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L15
  %31 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L15
  store i32 123, ptr %i, align 4
  %32 = load i32, ptr %i, align 4
  %33 = and i32 %32, 5
  store i32 %33, ptr %i, align 4
  %34 = load i32, ptr %i, align 4
  %35 = icmp eq i32 %34, 1
  br i1 %35, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L19
  %36 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L19
  store i32 123, ptr %i, align 4
  %37 = load i32, ptr %i, align 4
  %38 = or i32 %37, 5
  store i32 %38, ptr %i, align 4
  %39 = load i32, ptr %i, align 4
  %40 = icmp eq i32 %39, 127
  br i1 %40, label %assert.exit.L27, label %assert.then.L27, !prof !0

assert.then.L27:                                  ; preds = %assert.exit.L23
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L23
  %42 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %43 = load i32, ptr %result, align 4
  ret i32 %43
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
