; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Struct__intref_bool = type { ptr, i1 }

@printf.str.0 = private unnamed_addr constant [20 x i8] c"From procedure: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"From function: %d\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 'i == -4309' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'd == -107.64' evaluated to false.\00", align 1
@printf.str.2 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

define private void @_p__void__void__proc__intref_Structref(ptr %0, ptr %1) {
  %intRef = alloca ptr, align 8
  %structRef = alloca ptr, align 8
  store ptr %0, ptr %intRef, align 8
  store ptr %1, ptr %structRef, align 8
  %3 = load ptr, ptr %intRef, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add i32 %4, 12
  store i32 %5, ptr %3, align 4
  %6 = load ptr, ptr %structRef, align 8
  %ref_addr = getelementptr inbounds %__Struct__intref_bool, ptr %6, i32 0, i32 0
  %7 = load ptr, ptr %ref_addr, align 8
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  ret void
}

declare i32 @printf(ptr noundef, ...)

define private i32 @_f__void__int__func__doubleref_Structref(ptr %0, ptr %1) {
  %result = alloca i32, align 4
  %doubleRef = alloca ptr, align 8
  %structRef = alloca ptr, align 8
  store ptr %0, ptr %doubleRef, align 8
  store ptr %1, ptr %structRef, align 8
  %3 = load ptr, ptr %doubleRef, align 8
  %4 = load double, ptr %3, align 8
  %5 = fmul double %4, -1.560000e+00
  store double %5, ptr %3, align 8
  %6 = load ptr, ptr %structRef, align 8
  %b_addr = getelementptr inbounds %__Struct__intref_bool, ptr %6, i32 0, i32 1
  %7 = load i1, ptr %b_addr, align 1
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %8)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca %__Struct__intref_bool, align 8
  %d = alloca double, align 8
  %2 = alloca %__Struct__intref_bool, align 8
  store i32 0, ptr %result, align 4
  store i32 -4321, ptr %i, align 4
  %3 = getelementptr inbounds %__Struct__intref_bool, ptr %1, i32 0, i32 0
  store ptr %i, ptr %3, align 8
  %4 = getelementptr inbounds %__Struct__intref_bool, ptr %1, i32 0, i32 1
  store i1 true, ptr %4, align 1
  call void @_p__void__void__proc__intref_Structref(ptr %i, ptr %1)
  %5 = load i32, ptr %i, align 4
  %6 = icmp eq i32 %5, -4309
  br i1 %6, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %0
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %0
  store double 6.900000e+01, ptr %d, align 8
  %8 = getelementptr inbounds %__Struct__intref_bool, ptr %2, i32 0, i32 0
  store ptr %i, ptr %8, align 8
  %9 = getelementptr inbounds %__Struct__intref_bool, ptr %2, i32 0, i32 1
  store i1 false, ptr %9, align 1
  %10 = call i32 @_f__void__int__func__doubleref_Structref(ptr %d, ptr %2)
  store i32 %10, ptr %result, align 4
  %11 = load double, ptr %d, align 8
  %12 = fcmp oeq double %11, -1.076400e+02
  br i1 %12, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L20
  %13 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L20
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

declare void @exit(i32)

attributes #0 = { noinline nounwind optnone uwtable }

!0 = !{!"branch_weights", i32 2000, i32 1}
