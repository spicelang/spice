; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*iPtr == 1234' evaluated to false.\0A\00", align 4
@anon.array.0 = private unnamed_addr constant [3 x i64] [i64 123, i64 456, i64 789]
@anon.string.1 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 456l' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 789l' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '*lPtr == 123l' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %l = alloca i64, align 8
  %lPtr = alloca ptr, align 8
  %iPtr = alloca ptr, align 8
  %lArr = alloca [3 x i64], align 8
  %lPtr1 = alloca ptr, align 8
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
  %5 = call ptr @__acrt_iob_func(i32 2)
  %6 = call i32 (ptr, ptr, ...) @fprintf(ptr %5, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %0
  store [3 x i64] [i64 123, i64 456, i64 789], ptr %lArr, align 8
  %7 = getelementptr inbounds [3 x i64], ptr %lArr, i64 0, i32 0
  store ptr %7, ptr %lPtr1, align 8
  %8 = load ptr, ptr %lPtr1, align 8
  %9 = load i64, ptr %8, align 8
  %10 = icmp eq i64 %9, 123
  br i1 %10, label %assert.exit.L17, label %assert.then.L17, !prof !5

assert.then.L17:                                  ; preds = %assert.exit.L10
  %11 = call ptr @__acrt_iob_func(i32 2)
  %12 = call i32 (ptr, ptr, ...) @fprintf(ptr %11, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L10
  %13 = load ptr, ptr %lPtr1, align 8
  %14 = getelementptr i64, ptr %13, i64 1
  store ptr %14, ptr %lPtr1, align 8
  %15 = load ptr, ptr %lPtr1, align 8
  %16 = load i64, ptr %15, align 8
  %17 = icmp eq i64 %16, 456
  br i1 %17, label %assert.exit.L21, label %assert.then.L21, !prof !5

assert.then.L21:                                  ; preds = %assert.exit.L17
  %18 = call ptr @__acrt_iob_func(i32 2)
  %19 = call i32 (ptr, ptr, ...) @fprintf(ptr %18, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L17
  %20 = load ptr, ptr %lPtr1, align 8
  %21 = getelementptr i64, ptr %20, i64 -1
  store ptr %21, ptr %lPtr1, align 8
  %22 = load ptr, ptr %lPtr1, align 8
  %23 = load i64, ptr %22, align 8
  %24 = icmp eq i64 %23, 123
  br i1 %24, label %assert.exit.L25, label %assert.then.L25, !prof !5

assert.then.L25:                                  ; preds = %assert.exit.L21
  %25 = call ptr @__acrt_iob_func(i32 2)
  %26 = call i32 (ptr, ptr, ...) @fprintf(ptr %25, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L21
  %27 = load ptr, ptr %lPtr1, align 8
  %28 = getelementptr i64, ptr %27, i64 2
  store ptr %28, ptr %lPtr1, align 8
  %29 = load ptr, ptr %lPtr1, align 8
  %30 = load i64, ptr %29, align 8
  %31 = icmp eq i64 %30, 789
  br i1 %31, label %assert.exit.L29, label %assert.then.L29, !prof !5

assert.then.L29:                                  ; preds = %assert.exit.L25
  %32 = call ptr @__acrt_iob_func(i32 2)
  %33 = call i32 (ptr, ptr, ...) @fprintf(ptr %32, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L25
  %34 = load ptr, ptr %lPtr1, align 8
  %35 = getelementptr i64, ptr %34, i64 -2
  store ptr %35, ptr %lPtr1, align 8
  %36 = load ptr, ptr %lPtr1, align 8
  %37 = load i64, ptr %36, align 8
  %38 = icmp eq i64 %37, 123
  br i1 %38, label %assert.exit.L33, label %assert.then.L33, !prof !5

assert.then.L33:                                  ; preds = %assert.exit.L29
  %39 = call ptr @__acrt_iob_func(i32 2)
  %40 = call i32 (ptr, ptr, ...) @fprintf(ptr %39, ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %41 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
