; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]
@anon.string.0 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 2' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 3' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %a = alloca [3 x i32], align 4
  %aPtr = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store [3 x i32] [i32 1, i32 2, i32 3], ptr %a, align 4
  %1 = getelementptr inbounds [3 x i32], ptr %a, i64 0, i32 0
  store ptr %1, ptr %aPtr, align 8
  %2 = load ptr, ptr %aPtr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %assert.exit.L4, label %assert.then.L4, !prof !5

assert.then.L4:                                   ; preds = %0
  %5 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %0
  %6 = load ptr, ptr %aPtr, align 8
  %7 = getelementptr i32, ptr %6, i64 1
  store ptr %7, ptr %aPtr, align 8
  %8 = load ptr, ptr %aPtr, align 8
  %9 = load i32, ptr %8, align 4
  %10 = icmp eq i32 %9, 2
  br i1 %10, label %assert.exit.L6, label %assert.then.L6, !prof !5

assert.then.L6:                                   ; preds = %assert.exit.L4
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %assert.exit.L4
  %12 = load ptr, ptr %aPtr, align 8
  %13 = getelementptr i32, ptr %12, i64 -1
  store ptr %13, ptr %aPtr, align 8
  %14 = load ptr, ptr %aPtr, align 8
  %15 = load i32, ptr %14, align 4
  %16 = icmp eq i32 %15, 1
  br i1 %16, label %assert.exit.L8, label %assert.then.L8, !prof !5

assert.then.L8:                                   ; preds = %assert.exit.L6
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %assert.exit.L6
  %18 = load ptr, ptr %aPtr, align 8
  %19 = getelementptr i32, ptr %18, i64 2
  store ptr %19, ptr %aPtr, align 8
  %20 = load ptr, ptr %aPtr, align 8
  %21 = load i32, ptr %20, align 4
  %22 = icmp eq i32 %21, 3
  br i1 %22, label %assert.exit.L10, label %assert.then.L10, !prof !5

assert.then.L10:                                  ; preds = %assert.exit.L8
  %23 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L8
  %24 = load ptr, ptr %aPtr, align 8
  %25 = getelementptr i32, ptr %24, i64 -2
  store ptr %25, ptr %aPtr, align 8
  %26 = load ptr, ptr %aPtr, align 8
  %27 = load i32, ptr %26, align 4
  %28 = icmp eq i32 %27, 1
  br i1 %28, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %assert.exit.L10
  %29 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L10
  %30 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %31 = load i32, ptr %result, align 4
  ret i32 %31
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
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
