; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [10 x i32] [i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1]
@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@printf.str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define private void @_Z4swapRiRi(ptr %0, ptr %1) {
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  %temp = alloca i32, align 4
  store ptr %0, ptr %a, align 8
  store ptr %1, ptr %b, align 8
  %3 = load ptr, ptr %a, align 8
  %4 = load i32, ptr %3, align 4
  store i32 %4, ptr %temp, align 4
  %5 = load ptr, ptr %a, align 8
  %6 = load ptr, ptr %b, align 8
  %7 = load i32, ptr %6, align 4
  store i32 %7, ptr %5, align 4
  %8 = load ptr, ptr %b, align 8
  %9 = load i32, ptr %temp, align 4
  store i32 %9, ptr %8, align 4
  ret void
}

define private void @_Z4sortRA10_iPFbiiE(ptr %0, { ptr, ptr } %1) {
  %array = alloca ptr, align 8
  %sortFct = alloca { ptr, ptr }, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store ptr %0, ptr %array, align 8
  store { ptr, ptr } %1, ptr %sortFct, align 8
  store i32 0, ptr %i, align 4
  br label %for.head.L8

for.head.L8:                                      ; preds = %for.tail.L8, %2
  %3 = load i32, ptr %i, align 4
  %4 = sext i32 %3 to i64
  %5 = icmp slt i64 %4, 9
  br i1 %5, label %for.body.L8, label %for.exit.L8

for.body.L8:                                      ; preds = %for.head.L8
  store i32 0, ptr %j, align 4
  br label %for.head.L9

for.head.L9:                                      ; preds = %for.tail.L9, %for.body.L8
  %6 = load i32, ptr %i, align 4
  %7 = sext i32 %6 to i64
  %8 = sub nsw i64 10, %7
  %9 = sub nsw i64 %8, 1
  %10 = load i32, ptr %j, align 4
  %11 = sext i32 %10 to i64
  %12 = icmp slt i64 %11, %9
  br i1 %12, label %for.body.L9, label %for.exit.L9

for.body.L9:                                      ; preds = %for.head.L9
  %13 = load i32, ptr %j, align 4
  %14 = load ptr, ptr %array, align 8
  %15 = getelementptr inbounds [10 x i32], ptr %14, i64 0, i32 %13
  %16 = load i32, ptr %15, align 4
  %17 = load i32, ptr %j, align 4
  %18 = add nsw i32 %17, 1
  %19 = load ptr, ptr %array, align 8
  %20 = getelementptr inbounds [10 x i32], ptr %19, i64 0, i32 %18
  %21 = load i32, ptr %20, align 4
  %fct = load ptr, ptr %sortFct, align 8
  %22 = call i1 %fct(i32 %16, i32 %21)
  br i1 %22, label %if.then.L10, label %if.exit.L10

if.then.L10:                                      ; preds = %for.body.L9
  %23 = load i32, ptr %j, align 4
  %24 = load ptr, ptr %array, align 8
  %25 = getelementptr inbounds [10 x i32], ptr %24, i64 0, i32 %23
  %26 = load i32, ptr %j, align 4
  %27 = add nsw i32 %26, 1
  %28 = load ptr, ptr %array, align 8
  %29 = getelementptr inbounds [10 x i32], ptr %28, i64 0, i32 %27
  call void @_Z4swapRiRi(ptr %25, ptr %29)
  br label %if.exit.L10

if.exit.L10:                                      ; preds = %if.then.L10, %for.body.L9
  br label %for.tail.L9

for.tail.L9:                                      ; preds = %if.exit.L10
  %30 = load i32, ptr %j, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %j, align 4
  br label %for.head.L9

for.exit.L9:                                      ; preds = %for.head.L9
  br label %for.tail.L8

for.tail.L8:                                      ; preds = %for.exit.L9
  %32 = load i32, ptr %i, align 4
  %33 = add nsw i32 %32, 1
  store i32 %33, ptr %i, align 4
  br label %for.head.L8

for.exit.L8:                                      ; preds = %for.head.L8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %array = alloca [10 x i32], align 4
  %fat.ptr = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store [10 x i32] [i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1], ptr %array, align 4
  store ptr @_Z15lambda.L19C17.0ii, ptr %fat.ptr, align 8
  %1 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %1, align 8
  %2 = load { ptr, ptr }, ptr %fat.ptr, align 8
  call void @_Z4sortRA10_iPFbiiE(ptr %array, { ptr, ptr } %2)
  call void @_Z10printArrayRA10_i(ptr %array)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

define private i1 @_Z15lambda.L19C17.0ii(i32 %0, i32 %1) {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %a, align 4
  %4 = load i32, ptr %b, align 4
  %5 = icmp sgt i32 %3, %4
  ret i1 %5
}

define private void @_Z10printArrayRA10_i(ptr %0) {
  %array = alloca ptr, align 8
  %i = alloca i32, align 4
  store ptr %0, ptr %array, align 8
  store i32 0, ptr %i, align 4
  br label %for.head.L24

for.head.L24:                                     ; preds = %for.tail.L24, %1
  %2 = load i32, ptr %i, align 4
  %3 = sext i32 %2 to i64
  %4 = icmp slt i64 %3, 10
  br i1 %4, label %for.body.L24, label %for.exit.L24

for.body.L24:                                     ; preds = %for.head.L24
  %5 = load i32, ptr %i, align 4
  %6 = load ptr, ptr %array, align 8
  %7 = getelementptr inbounds [10 x i32], ptr %6, i64 0, i32 %5
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  br label %for.tail.L24

for.tail.L24:                                     ; preds = %for.body.L24
  %10 = load i32, ptr %i, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %i, align 4
  br label %for.head.L24

for.exit.L24:                                     ; preds = %for.head.L24
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
