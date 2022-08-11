; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Array1:\0A\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1
@2 = private unnamed_addr constant [9 x i8] c"Array2:\0A\00", align 1
@3 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@6 = private unnamed_addr constant [10 x i8] c"Item: %s\0A\00", align 1
@7 = private unnamed_addr constant [9 x i8] c"Array3:\0A\00", align 1
@8 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1
@9 = private unnamed_addr constant [9 x i8] c"Array4:\0A\00", align 1
@10 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %arraySize1 = alloca i16, align 2
  %array1 = alloca ptr, align 8
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  %arraySize2 = alloca i64, align 8
  %array2 = alloca ptr, align 8
  %idx2 = alloca i32, align 4
  %item3 = alloca ptr, align 8
  %arraySize3 = alloca i32, align 4
  %array3 = alloca ptr, align 8
  %idx6 = alloca i32, align 4
  %item7 = alloca i32, align 4
  %arraySize4 = alloca i32, align 4
  %array4 = alloca ptr, align 8
  %array410 = alloca ptr, align 8
  %idx11 = alloca i32, align 4
  %item12 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = call i32 (ptr, ...) @printf(ptr @0)
  store i16 4, ptr %arraySize1, align 2
  %1 = load i16, ptr %arraySize1, align 2
  %2 = call ptr @llvm.stacksave()
  %3 = alloca i32, i16 %1, align 4
  %4 = getelementptr inbounds i32, ptr %3, i32 0
  store i32 1, ptr %4, align 4
  %5 = getelementptr inbounds i32, ptr %3, i32 1
  store i32 2, ptr %5, align 4
  %6 = getelementptr inbounds i32, ptr %3, i32 2
  store i32 3, ptr %6, align 4
  store ptr %3, ptr %array1, align 8
  %7 = load ptr, ptr %array1, align 8
  %8 = getelementptr inbounds i32, ptr %7, i32 3
  store i32 0, ptr %8, align 4
  store i32 0, ptr %idx, align 4
  store i32 0, ptr %item, align 4
  %9 = load ptr, ptr %array1, align 8
  %10 = sext i16 %1 to i32
  %11 = load i32, ptr %idx, align 4
  %12 = load ptr, ptr %array1, align 8
  %13 = getelementptr inbounds i32, ptr %12, i32 %11
  %14 = load i32, ptr %13, align 4
  store i32 %14, ptr %item, align 4
  br label %foreach.loop.l6

foreach.loop.l6:                                  ; preds = %foreach.cond.l6, %entry.l1
  %15 = load i32, ptr %item, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @1, i32 %15)
  br label %foreach.inc.l6

foreach.inc.l6:                                   ; preds = %foreach.loop.l6
  %idx1 = load i32, ptr %idx, align 4
  %idx.inc = add i32 %idx1, 1
  store i32 %idx.inc, ptr %idx, align 4
  %17 = getelementptr inbounds i32, ptr %12, i32 %idx.inc
  %18 = load i32, ptr %17, align 4
  store i32 %18, ptr %item, align 4
  br label %foreach.cond.l6

foreach.cond.l6:                                  ; preds = %foreach.inc.l6
  %19 = load i32, ptr %idx, align 4
  %20 = icmp ult i32 %19, %10
  br i1 %20, label %foreach.loop.l6, label %foreach.end.l6

foreach.end.l6:                                   ; preds = %foreach.cond.l6
  %21 = call i32 (ptr, ...) @printf(ptr @2)
  store i64 2, ptr %arraySize2, align 8
  %22 = load i64, ptr %arraySize2, align 8
  %23 = alloca ptr, i64 %22, align 8
  store ptr %23, ptr %array2, align 8
  %24 = load ptr, ptr %array2, align 8
  %25 = getelementptr inbounds ptr, ptr %24, i32 0
  store ptr @3, ptr %25, align 8
  %26 = load ptr, ptr %array2, align 8
  %27 = getelementptr inbounds ptr, ptr %26, i32 1
  store ptr @4, ptr %27, align 8
  store i32 0, ptr %idx2, align 4
  store ptr @5, ptr %item3, align 8
  %28 = load ptr, ptr %array2, align 8
  %29 = trunc i64 %22 to i32
  %30 = load i32, ptr %idx2, align 4
  %31 = load ptr, ptr %array2, align 8
  %32 = getelementptr inbounds ptr, ptr %31, i32 %30
  %33 = load ptr, ptr %32, align 8
  store ptr %33, ptr %item3, align 8
  br label %foreach.loop.l15

foreach.loop.l15:                                 ; preds = %foreach.cond.l15, %foreach.end.l6
  %34 = load ptr, ptr %item3, align 8
  %35 = call i32 (ptr, ...) @printf(ptr @6, ptr %34)
  br label %foreach.inc.l15

foreach.inc.l15:                                  ; preds = %foreach.loop.l15
  %idx4 = load i32, ptr %idx2, align 4
  %idx.inc5 = add i32 %idx4, 1
  store i32 %idx.inc5, ptr %idx2, align 4
  %36 = getelementptr inbounds ptr, ptr %31, i32 %idx.inc5
  %37 = load ptr, ptr %36, align 8
  store ptr %37, ptr %item3, align 8
  br label %foreach.cond.l15

foreach.cond.l15:                                 ; preds = %foreach.inc.l15
  %38 = load i32, ptr %idx2, align 4
  %39 = icmp ult i32 %38, %29
  br i1 %39, label %foreach.loop.l15, label %foreach.end.l15

foreach.end.l15:                                  ; preds = %foreach.cond.l15
  %40 = call i32 (ptr, ...) @printf(ptr @7)
  store i32 3, ptr %arraySize3, align 4
  %41 = load i32, ptr %arraySize3, align 4
  %42 = load i32, ptr %arraySize3, align 4
  %43 = load i32, ptr %arraySize3, align 4
  %44 = alloca i32, i32 %41, align 4
  %45 = getelementptr inbounds i32, ptr %44, i32 0
  store i32 1, ptr %45, align 4
  %46 = getelementptr inbounds i32, ptr %44, i32 1
  store i32 %42, ptr %46, align 4
  %47 = getelementptr inbounds i32, ptr %44, i32 2
  store i32 %43, ptr %47, align 4
  store ptr %44, ptr %array3, align 8
  store i32 0, ptr %idx6, align 4
  store i32 0, ptr %item7, align 4
  %48 = load ptr, ptr %array3, align 8
  %49 = load i32, ptr %idx6, align 4
  %50 = load ptr, ptr %array3, align 8
  %51 = getelementptr inbounds i32, ptr %50, i32 %49
  %52 = load i32, ptr %51, align 4
  store i32 %52, ptr %item7, align 4
  br label %foreach.loop.l22

foreach.loop.l22:                                 ; preds = %foreach.cond.l22, %foreach.end.l15
  %53 = load i32, ptr %item7, align 4
  %54 = call i32 (ptr, ...) @printf(ptr @8, i32 %53)
  br label %foreach.inc.l22

foreach.inc.l22:                                  ; preds = %foreach.loop.l22
  %idx8 = load i32, ptr %idx6, align 4
  %idx.inc9 = add i32 %idx8, 1
  store i32 %idx.inc9, ptr %idx6, align 4
  %55 = getelementptr inbounds i32, ptr %50, i32 %idx.inc9
  %56 = load i32, ptr %55, align 4
  store i32 %56, ptr %item7, align 4
  br label %foreach.cond.l22

foreach.cond.l22:                                 ; preds = %foreach.inc.l22
  %57 = load i32, ptr %idx6, align 4
  %58 = icmp ult i32 %57, %41
  br i1 %58, label %foreach.loop.l22, label %foreach.end.l22

foreach.end.l22:                                  ; preds = %foreach.cond.l22
  %59 = call i32 (ptr, ...) @printf(ptr @9)
  store i32 3, ptr %arraySize4, align 4
  %60 = load i32, ptr %arraySize4, align 4
  %61 = alloca i32, i32 %60, align 4
  store ptr %61, ptr %array4, align 8
  %62 = load i32, ptr %arraySize4, align 4
  %63 = load i32, ptr %arraySize4, align 4
  %64 = alloca i32, align 4
  %65 = getelementptr inbounds i32, ptr %64, i32 0
  store i32 1, ptr %65, align 4
  %66 = getelementptr inbounds i32, ptr %64, i32 1
  store i32 %62, ptr %66, align 4
  %67 = getelementptr inbounds i32, ptr %64, i32 2
  store i32 %63, ptr %67, align 4
  store ptr %64, ptr %array410, align 8
  %68 = load ptr, ptr %array410, align 8
  store ptr %68, ptr %array4, align 8
  store i32 0, ptr %idx11, align 4
  store i32 0, ptr %item12, align 4
  %69 = load ptr, ptr %array4, align 8
  %70 = load i32, ptr %idx11, align 4
  %71 = load ptr, ptr %array4, align 8
  %72 = getelementptr inbounds i32, ptr %71, i32 %70
  %73 = load i32, ptr %72, align 4
  store i32 %73, ptr %item12, align 4
  br label %foreach.loop.l30

foreach.loop.l30:                                 ; preds = %foreach.cond.l30, %foreach.end.l22
  %74 = load i32, ptr %item12, align 4
  %75 = call i32 (ptr, ...) @printf(ptr @10, i32 %74)
  br label %foreach.inc.l30

foreach.inc.l30:                                  ; preds = %foreach.loop.l30
  %idx13 = load i32, ptr %idx11, align 4
  %idx.inc14 = add i32 %idx13, 1
  store i32 %idx.inc14, ptr %idx11, align 4
  %76 = getelementptr inbounds i32, ptr %71, i32 %idx.inc14
  %77 = load i32, ptr %76, align 4
  store i32 %77, ptr %item12, align 4
  br label %foreach.cond.l30

foreach.cond.l30:                                 ; preds = %foreach.inc.l30
  %78 = load i32, ptr %idx11, align 4
  %79 = icmp ult i32 %78, %60
  br i1 %79, label %foreach.loop.l30, label %foreach.end.l30

foreach.end.l30:                                  ; preds = %foreach.cond.l30
  call void @llvm.stackrestore(ptr %2)
  %80 = load i32, ptr %result, align 4
  ret i32 %80
}

declare i32 @printf(ptr, ...)

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave() #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore(ptr) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn }
