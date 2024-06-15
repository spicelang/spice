; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@anon.array.0 = private unnamed_addr constant [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@anon.array.1 = private unnamed_addr constant [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define private i32 @_Z10sumNumbersIsEiPsl(ptr %0, i64 %1) {
  %result = alloca i32, align 4
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i64, align 8
  %i = alloca i64, align 8
  store ptr %0, ptr %numberArray, align 8
  store i64 %1, ptr %arrayLength, align 8
  store i32 0, ptr %result, align 4
  store i64 0, ptr %i, align 8
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %2
  %3 = load i64, ptr %arrayLength, align 8
  %4 = load i64, ptr %i, align 8
  %5 = icmp slt i64 %4, %3
  br i1 %5, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %6 = load i64, ptr %i, align 8
  %7 = load ptr, ptr %numberArray, align 8
  %8 = getelementptr inbounds i16, ptr %7, i64 %6
  %9 = load i16, ptr %8, align 2
  %10 = sext i16 %9 to i32
  %11 = load i32, ptr %result, align 4
  %12 = add nsw i32 %11, %10
  store i32 %12, ptr %result, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %13 = load i64, ptr %i, align 8
  %14 = add nsw i64 %13, 1
  store i64 %14, ptr %i, align 8
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

define private i32 @_Z10sumNumbersIlEiPll(ptr %0, i64 %1) {
  %result = alloca i32, align 4
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i64, align 8
  %i = alloca i64, align 8
  store ptr %0, ptr %numberArray, align 8
  store i64 %1, ptr %arrayLength, align 8
  store i32 0, ptr %result, align 4
  store i64 0, ptr %i, align 8
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %2
  %3 = load i64, ptr %arrayLength, align 8
  %4 = load i64, ptr %i, align 8
  %5 = icmp slt i64 %4, %3
  br i1 %5, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %6 = load i64, ptr %i, align 8
  %7 = load ptr, ptr %numberArray, align 8
  %8 = getelementptr inbounds i64, ptr %7, i64 %6
  %9 = load i64, ptr %8, align 8
  %10 = trunc i64 %9 to i32
  %11 = load i32, ptr %result, align 4
  %12 = add nsw i32 %11, %10
  store i32 %12, ptr %result, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %13 = load i64, ptr %i, align 8
  %14 = add nsw i64 %13, 1
  store i64 %14, ptr %i, align 8
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

define private void @_Z9printDataIPiEvlPi(i64 %0, [2 x i32] %1) {
  %arrayLength = alloca i64, align 8
  %list = alloca [2 x i32], align 4
  %i = alloca i64, align 8
  store i64 %0, ptr %arrayLength, align 8
  store [2 x i32] %1, ptr %list, align 4
  store i64 0, ptr %i, align 8
  br label %for.head.L12

for.head.L12:                                     ; preds = %for.tail.L12, %2
  %3 = load i64, ptr %arrayLength, align 8
  %4 = load i64, ptr %i, align 8
  %5 = icmp slt i64 %4, %3
  br i1 %5, label %for.body.L12, label %for.exit.L12

for.body.L12:                                     ; preds = %for.head.L12
  %6 = load i64, ptr %i, align 8
  %7 = getelementptr inbounds [2 x i32], ptr %list, i64 0, i64 %6
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  br label %for.tail.L12

for.tail.L12:                                     ; preds = %for.body.L12
  %10 = load i64, ptr %i, align 8
  %11 = add nsw i64 %10, 1
  store i64 %11, ptr %i, align 8
  br label %for.head.L12

for.exit.L12:                                     ; preds = %for.head.L12
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %numberList1 = alloca [7 x i16], align 2
  %result1 = alloca i32, align 4
  %numberList2 = alloca [4 x i64], align 8
  %result2 = alloca i32, align 4
  %1 = alloca [2 x i32], align 4
  %resultList = alloca [2 x i32], align 4
  store i32 0, ptr %result, align 4
  store [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7], ptr %numberList1, align 2
  %2 = getelementptr inbounds [7 x i16], ptr %numberList1, i64 0, i32 0
  %3 = call i32 @_Z10sumNumbersIsEiPsl(ptr %2, i64 7)
  store i32 %3, ptr %result1, align 4
  store [4 x i64] [i64 10, i64 12, i64 14, i64 16], ptr %numberList2, align 8
  %4 = getelementptr inbounds [4 x i64], ptr %numberList2, i64 0, i32 0
  %5 = call i32 @_Z10sumNumbersIlEiPll(ptr %4, i64 4)
  store i32 %5, ptr %result2, align 4
  %6 = getelementptr inbounds [2 x i32], ptr %1, i64 0
  %7 = load i32, ptr %result1, align 4
  store i32 %7, ptr %6, align 4
  %8 = load i32, ptr %result2, align 4
  %9 = getelementptr inbounds i32, ptr %6, i64 1
  store i32 %8, ptr %9, align 4
  %10 = load [2 x i32], ptr %1, align 4
  store [2 x i32] %10, ptr %resultList, align 4
  %11 = load [2 x i32], ptr %resultList, align 4
  call void @_Z9printDataIPiEvlPi(i64 2, [2 x i32] %11)
  %12 = load i32, ptr %result1, align 4
  %13 = load i32, ptr %result2, align 4
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %12, i32 %13)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
