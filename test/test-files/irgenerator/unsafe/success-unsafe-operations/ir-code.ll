; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*iPtr == 1234' evaluated to false.\0A\00", align 4
@anon.array.0 = private unnamed_addr constant [3 x i64] [i64 123, i64 456, i64 789]
@anon.string.1 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 456l' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 789l' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %l = alloca i64, align 8
  %lPtr = alloca ptr, align 8
  %iPtr = alloca ptr, align 8
  %lArr = alloca [3 x i64], align 8
  %lPtr1 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i64 1234, ptr %l, align 8
  store ptr %l, ptr %lPtr, align 8
  store ptr null, ptr %iPtr, align 8
  %1 = load ptr, ptr %lPtr, align 8
  store ptr %1, ptr %iPtr, align 8
  %2 = load ptr, ptr %iPtr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 1234
  br i1 %4, label %assert.exit.L10, label %assert.then.L10, !prof !5

assert.then.L10:                                  ; preds = %0
  %5 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %0
  store [3 x i64] [i64 123, i64 456, i64 789], ptr %lArr, align 8
  %6 = getelementptr inbounds [3 x i64], ptr %lArr, i64 0, i32 0
  store ptr %6, ptr %lPtr1, align 8
  %7 = load ptr, ptr %lPtr1, align 8
  %8 = load i64, ptr %7, align 8
  %9 = icmp eq i64 %8, 123
  br i1 %9, label %assert.exit.L17, label %assert.then.L17, !prof !5

assert.then.L17:                                  ; preds = %assert.exit.L10
  %10 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L10
  %11 = load ptr, ptr %lPtr1, align 8
  %12 = getelementptr i64, ptr %11, i64 1
  store ptr %12, ptr %lPtr1, align 8
  %13 = load ptr, ptr %lPtr1, align 8
  %14 = load i64, ptr %13, align 8
  %15 = icmp eq i64 %14, 456
  br i1 %15, label %assert.exit.L21, label %assert.then.L21, !prof !5

assert.then.L21:                                  ; preds = %assert.exit.L17
  %16 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L17
  %17 = load ptr, ptr %lPtr1, align 8
  %18 = getelementptr i64, ptr %17, i64 -1
  store ptr %18, ptr %lPtr1, align 8
  %19 = load ptr, ptr %lPtr1, align 8
  %20 = load i64, ptr %19, align 8
  %21 = icmp eq i64 %20, 123
  br i1 %21, label %assert.exit.L25, label %assert.then.L25, !prof !5

assert.then.L25:                                  ; preds = %assert.exit.L21
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L21
  %23 = load ptr, ptr %lPtr1, align 8
  %24 = getelementptr i64, ptr %23, i64 2
  store ptr %24, ptr %lPtr1, align 8
  %25 = load ptr, ptr %lPtr1, align 8
  %26 = load i64, ptr %25, align 8
  %27 = icmp eq i64 %26, 789
  br i1 %27, label %assert.exit.L29, label %assert.then.L29, !prof !5

assert.then.L29:                                  ; preds = %assert.exit.L25
  %28 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L25
  %29 = load ptr, ptr %lPtr1, align 8
  %30 = getelementptr i64, ptr %29, i64 -2
  store ptr %30, ptr %lPtr1, align 8
  %31 = load ptr, ptr %lPtr1, align 8
  %32 = load i64, ptr %31, align 8
  %33 = icmp eq i64 %32, 123
  br i1 %33, label %assert.exit.L33, label %assert.then.L33, !prof !5

assert.then.L33:                                  ; preds = %assert.exit.L29
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %35 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %36 = load i32, ptr %result, align 4
  ret i32 %36
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
!5 = !{!"branch_weights", i32 1048575, i32 1}
