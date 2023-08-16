; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%anon.captures.0 = type { i32, i32 }
%anon.captures.0.0 = type { i32, i32 }

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'x == 6' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'foo2(x)' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'x == 11' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [19 x i8] c"All tests passed!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %z = alloca i32, align 4
  %w = alloca i32, align 4
  %captures = alloca %anon.captures.0, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %foo1 = alloca ptr, align 8
  %captures1 = alloca %anon.captures.0.0, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  %foo2 = alloca ptr, align 8
  %x = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %z, align 4
  store i32 3, ptr %w, align 4
  %1 = load i32, ptr %w, align 4
  %2 = getelementptr inbounds %anon.captures.0, ptr %captures, i32 0, i32 0
  store i32 %1, ptr %2, align 4
  %3 = load i32, ptr %z, align 4
  %4 = getelementptr inbounds %anon.captures.0, ptr %captures, i32 0, i32 1
  store i32 %3, ptr %4, align 4
  %5 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z14lambda.L4C20.0Ri, ptr %5, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %6, align 8
  store ptr %fat.ptr, ptr %foo1, align 8
  %7 = load i32, ptr %w, align 4
  %8 = getelementptr inbounds %anon.captures.0.0, ptr %captures1, i32 0, i32 0
  store i32 %7, ptr %8, align 4
  %9 = load i32, ptr %z, align 4
  %10 = getelementptr inbounds %anon.captures.0.0, ptr %captures1, i32 0, i32 1
  store i32 %9, ptr %10, align 4
  %11 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 0
  store ptr @_Z14lambda.L7C26.0Ri, ptr %11, align 8
  %12 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %12, align 8
  store ptr %fat.ptr2, ptr %foo2, align 8
  store i32 1, ptr %x, align 4
  %13 = load ptr, ptr %foo1, align 8
  %14 = getelementptr inbounds { ptr, ptr }, ptr %13, i32 0, i32 0
  %15 = getelementptr inbounds { ptr, ptr }, ptr %13, i32 0, i32 1
  %16 = load ptr, ptr %15, align 8
  %17 = load ptr, ptr %14, align 8
  call void %17(ptr %16, ptr %x)
  %18 = load i32, ptr %x, align 4
  %19 = icmp eq i32 %18, 6
  br i1 %19, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %0
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %0
  %21 = load ptr, ptr %foo2, align 8
  %22 = getelementptr inbounds { ptr, ptr }, ptr %21, i32 0, i32 0
  %23 = getelementptr inbounds { ptr, ptr }, ptr %21, i32 0, i32 1
  %24 = load ptr, ptr %23, align 8
  %25 = load ptr, ptr %22, align 8
  %26 = call i1 %25(ptr %24, ptr %x)
  br i1 %26, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %27 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %28 = load i32, ptr %x, align 4
  %29 = icmp eq i32 %28, 11
  br i1 %29, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %31 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %32 = load i32, ptr %result, align 4
  ret i32 %32
}

define private void @_Z14lambda.L4C20.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %4 = getelementptr inbounds %anon.captures.0, ptr %3, i32 0, i32 0
  %5 = getelementptr inbounds %anon.captures.0, ptr %3, i32 0, i32 1
  %6 = load i32, ptr %4, align 4
  %7 = load i32, ptr %5, align 4
  %8 = add i32 %7, %6
  %9 = load ptr, ptr %x, align 8
  %10 = load i32, ptr %9, align 4
  %11 = add i32 %10, %8
  store i32 %11, ptr %9, align 4
  ret void
}

define private i1 @_Z14lambda.L7C26.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %result = alloca i1, align 1
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %4 = getelementptr inbounds %anon.captures.0.0, ptr %3, i32 0, i32 0
  %5 = getelementptr inbounds %anon.captures.0.0, ptr %3, i32 0, i32 1
  %6 = load i32, ptr %4, align 4
  %7 = load i32, ptr %5, align 4
  %8 = add i32 %7, %6
  %9 = load ptr, ptr %x, align 8
  %10 = load i32, ptr %9, align 4
  %11 = add i32 %10, %8
  store i32 %11, ptr %9, align 4
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
