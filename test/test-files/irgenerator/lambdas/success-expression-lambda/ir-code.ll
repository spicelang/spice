; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

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

define private void @_Z4sortRA10iPFbiiE(ptr %0, ptr %1) {
  %array = alloca ptr, align 8
  %sortFct = alloca ptr, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store ptr %0, ptr %array, align 8
  store ptr %1, ptr %sortFct, align 8
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
  %8 = sub i64 10, %7
  %9 = sub i64 %8, 1
  %10 = load i32, ptr %j, align 4
  %11 = sext i32 %10 to i64
  %12 = icmp slt i64 %11, %9
  br i1 %12, label %for.body.L9, label %for.exit.L9

for.body.L9:                                      ; preds = %for.head.L9
  %13 = load i32, ptr %j, align 4
  %14 = load ptr, ptr %array, align 8
  %15 = getelementptr inbounds [10 x i32], ptr %14, i32 0, i32 %13
  %16 = load i32, ptr %15, align 4
  %17 = load i32, ptr %j, align 4
  %18 = add i32 %17, 1
  %19 = load ptr, ptr %array, align 8
  %20 = getelementptr inbounds [10 x i32], ptr %19, i32 0, i32 %18
  %21 = load i32, ptr %20, align 4
  %22 = load ptr, ptr %sortFct, align 8
  %23 = call i1 %22(i32 %16, i32 %21)
  br i1 %23, label %if.then.L10, label %if.exit.L10

if.then.L10:                                      ; preds = %for.body.L9
  %24 = load i32, ptr %j, align 4
  %25 = load ptr, ptr %array, align 8
  %26 = getelementptr inbounds [10 x i32], ptr %25, i32 0, i32 %24
  %27 = load i32, ptr %j, align 4
  %28 = add i32 %27, 1
  %29 = load ptr, ptr %array, align 8
  %30 = getelementptr inbounds [10 x i32], ptr %29, i32 0, i32 %28
  call void @_Z4swapRiRi(ptr %26, ptr %30)
  br label %if.exit.L10

if.exit.L10:                                      ; preds = %if.then.L10, %for.body.L9
  br label %for.tail.L9

for.tail.L9:                                      ; preds = %if.exit.L10
  %31 = load i32, ptr %j, align 4
  %32 = add i32 %31, 1
  store i32 %32, ptr %j, align 4
  br label %for.head.L9

for.exit.L9:                                      ; preds = %for.head.L9
  br label %for.tail.L8

for.tail.L8:                                      ; preds = %for.exit.L9
  %33 = load i32, ptr %i, align 4
  %34 = add i32 %33, 1
  store i32 %34, ptr %i, align 4
  br label %for.head.L8

for.exit.L8:                                      ; preds = %for.head.L8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [10 x i32], align 4
  %array = alloca [10 x i32], align 4
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 40, i1 false)
  store [10 x i32] [i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1], ptr %array, align 4
  call void @_Z4sortRA10iPFbiiE(ptr %array, ptr @_Z15lambda.L19C17.0ii)
  call void @_Z10printArrayRA10i(ptr %array)
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

define private i1 @_Z15lambda.L19C17.0ii(i32 %0, i32 %1) {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = load i32, ptr %a, align 4
  %5 = icmp sgt i32 %4, %3
  ret i1 %5
}

define private void @_Z10printArrayRA10i(ptr %0) {
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
  %7 = getelementptr inbounds [10 x i32], ptr %6, i32 0, i32 %5
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  br label %for.tail.L24

for.tail.L24:                                     ; preds = %for.body.L24
  %10 = load i32, ptr %i, align 4
  %11 = add i32 %10, 1
  store i32 %11, ptr %i, align 4
  br label %for.head.L24

for.exit.L24:                                     ; preds = %for.head.L24
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
