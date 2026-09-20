; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\0A\00", align 4
@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [60 x i8] c"Assertion failed: Condition 'i == 127' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %i = alloca i32, align 4
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
  br i1 %14, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %0
  %15 = load ptr, ptr @stderr, align 8
  %16 = call i32 (ptr, ptr, ...) @fprintf(ptr %15, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %0
  %17 = load i32, ptr %i, align 4
  %18 = add nsw i32 %17, 494665727
  store i32 %18, ptr %i, align 4
  %19 = load i32, ptr %i, align 4
  %20 = sdiv i32 %19, 2
  store i32 %20, ptr %i, align 4
  %21 = load i32, ptr %i, align 4
  %22 = mul nsw i32 %21, 2
  store i32 %22, ptr %i, align 4
  %23 = load i32, ptr %i, align 4
  %24 = sub nsw i32 %23, 494665727
  store i32 %24, ptr %i, align 4
  %25 = load i32, ptr %i, align 4
  %26 = icmp eq i32 %25, 1
  br i1 %26, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L9
  %27 = load ptr, ptr @stderr, align 8
  %28 = call i32 (ptr, ptr, ...) @fprintf(ptr %27, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L9
  store i32 123, ptr %i, align 4
  %29 = load i32, ptr %i, align 4
  %30 = srem i32 %29, 2
  store i32 %30, ptr %i, align 4
  %31 = load i32, ptr %i, align 4
  %32 = icmp eq i32 %31, 1
  br i1 %32, label %assert.exit.L19, label %assert.then.L19, !prof !5

assert.then.L19:                                  ; preds = %assert.exit.L15
  %33 = load ptr, ptr @stderr, align 8
  %34 = call i32 (ptr, ptr, ...) @fprintf(ptr %33, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L15
  store i32 123, ptr %i, align 4
  %35 = load i32, ptr %i, align 4
  %36 = and i32 %35, 5
  store i32 %36, ptr %i, align 4
  %37 = load i32, ptr %i, align 4
  %38 = icmp eq i32 %37, 1
  br i1 %38, label %assert.exit.L23, label %assert.then.L23, !prof !5

assert.then.L23:                                  ; preds = %assert.exit.L19
  %39 = load ptr, ptr @stderr, align 8
  %40 = call i32 (ptr, ptr, ...) @fprintf(ptr %39, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L19
  store i32 123, ptr %i, align 4
  %41 = load i32, ptr %i, align 4
  %42 = or i32 %41, 5
  store i32 %42, ptr %i, align 4
  %43 = load i32, ptr %i, align 4
  %44 = icmp eq i32 %43, 127
  br i1 %44, label %assert.exit.L27, label %assert.then.L27, !prof !5

assert.then.L27:                                  ; preds = %assert.exit.L23
  %45 = load ptr, ptr @stderr, align 8
  %46 = call i32 (ptr, ptr, ...) @fprintf(ptr %45, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L23
  %47 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
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
