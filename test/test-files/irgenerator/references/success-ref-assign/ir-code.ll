; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition '&test == &testRef' evaluated to false.\0A\00", align 4
@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.1 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 135' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 124' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 123' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 127' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'test == 123' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %test = alloca i32, align 4
  %testRef = alloca ptr, align 8
  store i32 123, ptr %test, align 4
  store ptr %test, ptr %testRef, align 8
  %1 = load ptr, ptr %testRef, align 8
  %2 = icmp eq ptr %test, %1
  br i1 %2, label %assert.exit.L5, label %assert.then.L5, !prof !5

assert.then.L5:                                   ; preds = %0
  %3 = load ptr, ptr @stderr, align 8
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %0
  %5 = load ptr, ptr %testRef, align 8
  %6 = load i32, ptr %5, align 4
  %7 = add nsw i32 %6, 12
  store i32 %7, ptr %5, align 4
  %8 = load i32, ptr %test, align 4
  %9 = icmp eq i32 %8, 135
  br i1 %9, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %assert.exit.L5
  %10 = load ptr, ptr @stderr, align 8
  %11 = call i32 (ptr, ptr, ...) @fprintf(ptr %10, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L5
  %12 = load ptr, ptr %testRef, align 8
  %13 = load i32, ptr %12, align 4
  %14 = sub nsw i32 %13, 11
  store i32 %14, ptr %12, align 4
  %15 = load i32, ptr %test, align 4
  %16 = icmp eq i32 %15, 124
  br i1 %16, label %assert.exit.L11, label %assert.then.L11, !prof !5

assert.then.L11:                                  ; preds = %assert.exit.L9
  %17 = load ptr, ptr @stderr, align 8
  %18 = call i32 (ptr, ptr, ...) @fprintf(ptr %17, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L9
  %19 = load ptr, ptr %testRef, align 8
  store i32 123, ptr %19, align 4
  %20 = load i32, ptr %test, align 4
  %21 = icmp eq i32 %20, 123
  br i1 %21, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %assert.exit.L11
  %22 = load ptr, ptr @stderr, align 8
  %23 = call i32 (ptr, ptr, ...) @fprintf(ptr %22, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L11
  %24 = load ptr, ptr %testRef, align 8
  %25 = load i32, ptr %24, align 4
  %26 = add nsw i32 %25, 4
  %27 = load ptr, ptr %testRef, align 8
  store i32 %26, ptr %27, align 4
  %28 = load i32, ptr %test, align 4
  %29 = icmp eq i32 %28, 127
  br i1 %29, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L13
  %30 = load ptr, ptr @stderr, align 8
  %31 = call i32 (ptr, ptr, ...) @fprintf(ptr %30, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %32 = load ptr, ptr %testRef, align 8
  %33 = load i32, ptr %32, align 4
  %34 = sub nsw i32 %33, 4
  %35 = load ptr, ptr %testRef, align 8
  store i32 %34, ptr %35, align 4
  %36 = load i32, ptr %test, align 4
  %37 = icmp eq i32 %36, 123
  br i1 %37, label %assert.exit.L17, label %assert.then.L17, !prof !5

assert.then.L17:                                  ; preds = %assert.exit.L15
  %38 = load ptr, ptr @stderr, align 8
  %39 = call i32 (ptr, ptr, ...) @fprintf(ptr %38, ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %40 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
