; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [21 x i8] c"Sum after write: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [16 x i8] c"Matrix sum: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [12 x i8] c"Lambda: %d\0A\00", align 4

define private noundef i32 @_Z3sumA4_i(ptr noundef %values) {
  %result = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %total, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 4
  br i1 %2, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %3 = load i32, ptr %i, align 4
  %4 = getelementptr inbounds [4 x i32], ptr %values, i64 0, i32 %3
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %total, align 4
  %7 = add nsw i32 %6, %5
  store i32 %7, ptr %total, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %8 = load i32, ptr %i, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr %i, align 4
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %10 = load i32, ptr %total, align 4
  ret i32 %10
}

define private void @_Z8setFirstA4_ii(ptr noundef %values, i32 noundef %0) {
  %newValue = alloca i32, align 4
  store i32 %0, ptr %newValue, align 4
  %2 = getelementptr inbounds [4 x i32], ptr %values, i64 0, i32 0
  %3 = load i32, ptr %newValue, align 4
  store i32 %3, ptr %2, align 4
  ret void
}

define private noundef i32 @_Z9sumMatrixA3_A2_b(ptr noundef %matrix) {
  %result = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %total, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L18

for.head.L18:                                     ; preds = %for.tail.L18, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 3
  br i1 %2, label %for.body.L18, label %for.exit.L18

for.body.L18:                                     ; preds = %for.head.L18
  store i32 0, ptr %j, align 4
  br label %for.head.L19

for.head.L19:                                     ; preds = %for.tail.L19, %for.body.L18
  %3 = load i32, ptr %j, align 4
  %4 = icmp slt i32 %3, 2
  br i1 %4, label %for.body.L19, label %for.exit.L19

for.body.L19:                                     ; preds = %for.head.L19
  %5 = load i32, ptr %i, align 4
  %6 = getelementptr inbounds [3 x [2 x i1]], ptr %matrix, i64 0, i32 %5
  %7 = load i32, ptr %j, align 4
  %8 = getelementptr inbounds [2 x i1], ptr %6, i64 0, i32 %7
  %9 = load i1, ptr %8, align 1
  %10 = select i1 %9, i32 1, i32 0
  %11 = load i32, ptr %total, align 4
  %12 = add nsw i32 %11, %10
  store i32 %12, ptr %total, align 4
  br label %for.tail.L19

for.tail.L19:                                     ; preds = %for.body.L19
  %13 = load i32, ptr %j, align 4
  %14 = add nsw i32 %13, 1
  store i32 %14, ptr %j, align 4
  br label %for.head.L19

for.exit.L19:                                     ; preds = %for.head.L19
  br label %for.tail.L18

for.tail.L18:                                     ; preds = %for.exit.L19
  %15 = load i32, ptr %i, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr %i, align 4
  br label %for.head.L18

for.exit.L18:                                     ; preds = %for.head.L18
  %17 = load i32, ptr %total, align 4
  ret i32 %17
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %numbers = alloca [4 x i32], align 4
  %matrix = alloca [3 x [2 x i1]], align 1
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %lambda = alloca { ptr, ptr, i64 }, align 8
  store i32 0, ptr %result, align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %numbers, align 4
  %1 = call noundef i32 @_Z3sumA4_i(ptr noundef %numbers)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %1)
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef 4)
  call void @_Z8setFirstA4_ii(ptr noundef %numbers, i32 noundef 10)
  %4 = call noundef i32 @_Z3sumA4_i(ptr noundef %numbers)
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %4)
  store [3 x [2 x i1]] zeroinitializer, ptr %matrix, align 1
  %6 = getelementptr inbounds [3 x [2 x i1]], ptr %matrix, i64 0, i32 0
  %7 = getelementptr inbounds [2 x i1], ptr %6, i64 0, i32 0
  store i1 true, ptr %7, align 1
  %8 = getelementptr inbounds [3 x [2 x i1]], ptr %matrix, i64 0, i32 1
  %9 = getelementptr inbounds [2 x i1], ptr %8, i64 0, i32 1
  store i1 true, ptr %9, align 1
  %10 = getelementptr inbounds [3 x [2 x i1]], ptr %matrix, i64 0, i32 2
  %11 = getelementptr inbounds [2 x i1], ptr %10, i64 0, i32 0
  store i1 true, ptr %11, align 1
  %12 = call noundef i32 @_Z9sumMatrixA3_A2_b(ptr noundef %matrix)
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %12)
  store ptr @_Z15lambda.L42C29.0A4_i, ptr %fat.ptr, align 8
  %14 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr null, ptr %14, align 8
  %15 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 0, ptr %15, align 8
  %16 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  store { ptr, ptr, i64 } %16, ptr %lambda, align 8
  %17 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %lambda, i32 0, i32 1
  %captures = load ptr, ptr %17, align 8
  %fct = load ptr, ptr %lambda, align 8
  %18 = call i32 %fct(ptr %captures, ptr %numbers)
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %18)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

define private i32 @_Z15lambda.L42C29.0A4_i(ptr %0, ptr %values) {
  %result = alloca i32, align 4
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = getelementptr inbounds [4 x i32], ptr %values, i64 0, i32 3
  %3 = load i32, ptr %2, align 4
  ret i32 %3
}

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
