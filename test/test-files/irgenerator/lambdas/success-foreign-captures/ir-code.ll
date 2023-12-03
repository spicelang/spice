; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'x == 6' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 'l2(x)' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'x == 11' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [19 x i8] c"All tests passed!\0A\00", align 1

define private void @_Z4testPFCvRiEPFCbRiE({ ptr, ptr } %0, { ptr, ptr } %1) {
  %l1 = alloca { ptr, ptr }, align 8
  %l2 = alloca { ptr, ptr }, align 8
  %x = alloca i32, align 4
  store { ptr, ptr } %0, ptr %l1, align 8
  store { ptr, ptr } %1, ptr %l2, align 8
  store i32 1, ptr %x, align 4
  %3 = getelementptr inbounds { ptr, ptr }, ptr %l1, i32 0, i32 0
  %4 = getelementptr inbounds { ptr, ptr }, ptr %l1, i32 0, i32 1
  %captures = load ptr, ptr %4, align 8
  %fct = load ptr, ptr %3, align 8
  call void %fct(ptr %captures, ptr %x)
  %5 = load i32, ptr %x, align 4
  %6 = icmp eq i32 %5, 6
  br i1 %6, label %assert.exit.L4, label %assert.then.L4, !prof !0

assert.then.L4:                                   ; preds = %2
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %2
  %8 = getelementptr inbounds { ptr, ptr }, ptr %l2, i32 0, i32 0
  %9 = getelementptr inbounds { ptr, ptr }, ptr %l2, i32 0, i32 1
  %captures1 = load ptr, ptr %9, align 8
  %fct2 = load ptr, ptr %8, align 8
  %10 = call i1 %fct2(ptr %captures1, ptr %x)
  br i1 %10, label %assert.exit.L5, label %assert.then.L5, !prof !0

assert.then.L5:                                   ; preds = %assert.exit.L4
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %assert.exit.L4
  %12 = load i32, ptr %x, align 4
  %13 = icmp eq i32 %12, 11
  br i1 %13, label %assert.exit.L6, label %assert.then.L6, !prof !0

assert.then.L6:                                   ; preds = %assert.exit.L5
  %14 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %assert.exit.L5
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 {
  %result = alloca i32, align 4
  %z = alloca i32, align 4
  %w = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %foo1 = alloca { ptr, ptr }, align 8
  %captures1 = alloca { i32, i32 }, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  %foo2 = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store i32 2, ptr %z, align 4
  store i32 3, ptr %w, align 4
  %1 = load i32, ptr %w, align 4
  %2 = getelementptr inbounds { i32, i32 }, ptr %captures, i32 0, i32 0
  store i32 %1, ptr %2, align 4
  %3 = load i32, ptr %z, align 4
  %4 = getelementptr inbounds { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %3, ptr %4, align 4
  %5 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z15lambda.L12C20.0Ri, ptr %5, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %6, align 8
  %7 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %7, ptr %foo1, align 8
  %8 = load i32, ptr %w, align 4
  %9 = getelementptr inbounds { i32, i32 }, ptr %captures1, i32 0, i32 0
  store i32 %8, ptr %9, align 4
  %10 = load i32, ptr %z, align 4
  %11 = getelementptr inbounds { i32, i32 }, ptr %captures1, i32 0, i32 1
  store i32 %10, ptr %11, align 4
  %12 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 0
  store ptr @_Z15lambda.L15C26.0Ri, ptr %12, align 8
  %13 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %13, align 8
  %14 = load { ptr, ptr }, ptr %fat.ptr2, align 8
  store { ptr, ptr } %14, ptr %foo2, align 8
  %15 = load { ptr, ptr }, ptr %foo1, align 8
  %16 = load { ptr, ptr }, ptr %foo2, align 8
  call void @_Z4testPFCvRiEPFCbRiE({ ptr, ptr } %15, { ptr, ptr } %16)
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

define private void @_Z15lambda.L12C20.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %w = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 0
  %z = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %w, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret void
}

define private i1 @_Z15lambda.L15C26.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %result = alloca i1, align 1
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %w = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 0
  %z = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %w, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret i1 true
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { noinline nounwind optnone uwtable }

!0 = !{!"branch_weights", i32 2000, i32 1}
