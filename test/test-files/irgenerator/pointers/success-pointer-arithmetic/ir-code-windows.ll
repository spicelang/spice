; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]
@anon.string.0 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 2' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 3' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*aPtr == 1' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %a = alloca [3 x i32], align 4
  %aPtr = alloca ptr, align 8
  store [3 x i32] [i32 1, i32 2, i32 3], ptr %a, align 4
  %1 = getelementptr inbounds [3 x i32], ptr %a, i64 0, i32 0
  store ptr %1, ptr %aPtr, align 8
  %2 = load ptr, ptr %aPtr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %assert.exit.L4, label %assert.then.L4, !prof !5

assert.then.L4:                                   ; preds = %0
  %5 = call ptr @__acrt_iob_func(i32 2)
  %6 = call i32 (ptr, ptr, ...) @fprintf(ptr %5, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %0
  %7 = load ptr, ptr %aPtr, align 8
  %8 = getelementptr i32, ptr %7, i64 1
  store ptr %8, ptr %aPtr, align 8
  %9 = load ptr, ptr %aPtr, align 8
  %10 = load i32, ptr %9, align 4
  %11 = icmp eq i32 %10, 2
  br i1 %11, label %assert.exit.L6, label %assert.then.L6, !prof !5

assert.then.L6:                                   ; preds = %assert.exit.L4
  %12 = call ptr @__acrt_iob_func(i32 2)
  %13 = call i32 (ptr, ptr, ...) @fprintf(ptr %12, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %assert.exit.L4
  %14 = load ptr, ptr %aPtr, align 8
  %15 = getelementptr i32, ptr %14, i64 -1
  store ptr %15, ptr %aPtr, align 8
  %16 = load ptr, ptr %aPtr, align 8
  %17 = load i32, ptr %16, align 4
  %18 = icmp eq i32 %17, 1
  br i1 %18, label %assert.exit.L8, label %assert.then.L8, !prof !5

assert.then.L8:                                   ; preds = %assert.exit.L6
  %19 = call ptr @__acrt_iob_func(i32 2)
  %20 = call i32 (ptr, ptr, ...) @fprintf(ptr %19, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %assert.exit.L6
  %21 = load ptr, ptr %aPtr, align 8
  %22 = getelementptr i32, ptr %21, i64 2
  store ptr %22, ptr %aPtr, align 8
  %23 = load ptr, ptr %aPtr, align 8
  %24 = load i32, ptr %23, align 4
  %25 = icmp eq i32 %24, 3
  br i1 %25, label %assert.exit.L10, label %assert.then.L10, !prof !5

assert.then.L10:                                  ; preds = %assert.exit.L8
  %26 = call ptr @__acrt_iob_func(i32 2)
  %27 = call i32 (ptr, ptr, ...) @fprintf(ptr %26, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L8
  %28 = load ptr, ptr %aPtr, align 8
  %29 = getelementptr i32, ptr %28, i64 -2
  store ptr %29, ptr %aPtr, align 8
  %30 = load ptr, ptr %aPtr, align 8
  %31 = load i32, ptr %30, align 4
  %32 = icmp eq i32 %31, 1
  br i1 %32, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %assert.exit.L10
  %33 = call ptr @__acrt_iob_func(i32 2)
  %34 = call i32 (ptr, ptr, ...) @fprintf(ptr %33, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L10
  %35 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
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
