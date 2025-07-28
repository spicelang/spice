; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition '&test == &testRef' evaluated to false.\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 135' evaluated to false.\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 124' evaluated to false.\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 123' evaluated to false.\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 127' evaluated to false.\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 123' evaluated to false.\0A\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %testRef = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %test, align 4
  store ptr %test, ptr %testRef, align 8
  %1 = load ptr, ptr %testRef, align 8
  %2 = icmp eq ptr %test, %1
  br i1 %2, label %assert.exit.L5, label %assert.then.L5, !prof !5

assert.then.L5:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %0
  %4 = load ptr, ptr %testRef, align 8
  %5 = load i32, ptr %4, align 4
  %6 = add nsw i32 %5, 12
  store i32 %6, ptr %4, align 4
  %7 = load i32, ptr %test, align 4
  %8 = icmp eq i32 %7, 135
  br i1 %8, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %assert.exit.L5
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L5
  %10 = load ptr, ptr %testRef, align 8
  %11 = load i32, ptr %10, align 4
  %12 = sub nsw i32 %11, 11
  store i32 %12, ptr %10, align 4
  %13 = load i32, ptr %test, align 4
  %14 = icmp eq i32 %13, 124
  br i1 %14, label %assert.exit.L11, label %assert.then.L11, !prof !5

assert.then.L11:                                  ; preds = %assert.exit.L9
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L9
  %16 = load ptr, ptr %testRef, align 8
  store i32 123, ptr %16, align 4
  %17 = load i32, ptr %test, align 4
  %18 = icmp eq i32 %17, 123
  br i1 %18, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %assert.exit.L11
  %19 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L11
  %20 = load ptr, ptr %testRef, align 8
  %21 = load i32, ptr %20, align 4
  %22 = add nsw i32 %21, 4
  %23 = load ptr, ptr %testRef, align 8
  store i32 %22, ptr %23, align 4
  %24 = load i32, ptr %test, align 4
  %25 = icmp eq i32 %24, 127
  br i1 %25, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L13
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %27 = load ptr, ptr %testRef, align 8
  %28 = load i32, ptr %27, align 4
  %29 = sub nsw i32 %28, 4
  %30 = load ptr, ptr %testRef, align 8
  store i32 %29, ptr %30, align 4
  %31 = load i32, ptr %test, align 4
  %32 = icmp eq i32 %31, 123
  br i1 %32, label %assert.exit.L17, label %assert.then.L17, !prof !5

assert.then.L17:                                  ; preds = %assert.exit.L15
  %33 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %34 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %35 = load i32, ptr %result, align 4
  ret i32 %35
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
