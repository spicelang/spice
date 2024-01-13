; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'x == 6' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'foo2(x)' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'x == 11' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [19 x i8] c"All tests passed!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %z = alloca i32, align 4
  %w = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %foo1 = alloca { ptr, ptr }, align 8
  %captures1 = alloca { i32, i32 }, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  %foo2 = alloca { ptr, ptr }, align 8
  %x = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %z, align 4
  store i32 3, ptr %w, align 4
  %1 = load i32, ptr %w, align 4
  store i32 %1, ptr %captures, align 4
  %2 = load i32, ptr %z, align 4
  %3 = getelementptr inbounds { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %2, ptr %3, align 4
  store ptr @_Z14lambda.L4C20.0Ri, ptr %fat.ptr, align 8
  %4 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %4, align 8
  %5 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %5, ptr %foo1, align 8
  %6 = load i32, ptr %w, align 4
  store i32 %6, ptr %captures1, align 4
  %7 = load i32, ptr %z, align 4
  %8 = getelementptr inbounds { i32, i32 }, ptr %captures1, i32 0, i32 1
  store i32 %7, ptr %8, align 4
  store ptr @_Z14lambda.L7C26.0Ri, ptr %fat.ptr2, align 8
  %9 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %9, align 8
  %10 = load { ptr, ptr }, ptr %fat.ptr2, align 8
  store { ptr, ptr } %10, ptr %foo2, align 8
  store i32 1, ptr %x, align 4
  %11 = getelementptr inbounds { ptr, ptr }, ptr %foo1, i32 0, i32 1
  %captures3 = load ptr, ptr %11, align 8
  %fct = load ptr, ptr %foo1, align 8
  call void %fct(ptr %captures3, ptr %x)
  %12 = load i32, ptr %x, align 4
  %13 = icmp eq i32 %12, 6
  br i1 %13, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %0
  %14 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %0
  %15 = getelementptr inbounds { ptr, ptr }, ptr %foo2, i32 0, i32 1
  %captures4 = load ptr, ptr %15, align 8
  %fct5 = load ptr, ptr %foo2, align 8
  %16 = call i1 %fct5(ptr %captures4, ptr %x)
  br i1 %16, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %18 = load i32, ptr %x, align 4
  %19 = icmp eq i32 %18, 11
  br i1 %19, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %21 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %22 = load i32, ptr %result, align 4
  ret i32 %22
}

define private void @_Z14lambda.L4C20.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add nsw i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret void
}

define private i1 @_Z14lambda.L7C26.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %result = alloca i1, align 1
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add nsw i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret i1 true
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
