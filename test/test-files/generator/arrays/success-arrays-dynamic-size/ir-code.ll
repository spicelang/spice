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
  %array11 = alloca ptr, align 8
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  %arraySize2 = alloca i64, align 8
  %array2 = alloca ptr, align 8
  %idx3 = alloca i32, align 4
  %item4 = alloca ptr, align 8
  %arraySize3 = alloca i32, align 4
  %array3 = alloca ptr, align 8
  %array37 = alloca ptr, align 8
  %idx8 = alloca i32, align 4
  %item9 = alloca i32, align 4
  %arraySize4 = alloca i32, align 4
  %array4 = alloca ptr, align 8
  %array412 = alloca ptr, align 8
  %idx13 = alloca i32, align 4
  %item14 = alloca i32, align 4
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
  store ptr %7, ptr %array11, align 8
  %8 = load ptr, ptr %array11, align 8
  %9 = getelementptr inbounds i32, ptr %8, i32 3
  store i32 0, ptr %9, align 4
  store i32 0, ptr %idx, align 4
  store i32 0, ptr %item, align 4
  %10 = load ptr, ptr %array11, align 8
  %11 = sext i16 %1 to i32
  %12 = load i32, ptr %idx, align 4
  %13 = load ptr, ptr %array11, align 8
  %14 = getelementptr inbounds i32, ptr %13, i32 %12
  %15 = load i32, ptr %14, align 4
  store i32 %15, ptr %item, align 4
  br label %foreach.loop.l6

foreach.loop.l6:                                  ; preds = %foreach.cond.l6, %entry.l1
  %16 = load i32, ptr %item, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @1, i32 %16)
  br label %foreach.inc.l6

foreach.inc.l6:                                   ; preds = %foreach.loop.l6
  %idx2 = load i32, ptr %idx, align 4
  %idx.inc = add i32 %idx2, 1
  store i32 %idx.inc, ptr %idx, align 4
  %18 = getelementptr inbounds i32, ptr %13, i32 %idx.inc
  %19 = load i32, ptr %18, align 4
  store i32 %19, ptr %item, align 4
  br label %foreach.cond.l6

foreach.cond.l6:                                  ; preds = %foreach.inc.l6
  %20 = load i32, ptr %idx, align 4
  %21 = icmp ult i32 %20, %11
  br i1 %21, label %foreach.loop.l6, label %foreach.end.l6

foreach.end.l6:                                   ; preds = %foreach.cond.l6
  %22 = call i32 (ptr, ...) @printf(ptr @2)
  store i64 2, ptr %arraySize2, align 8
  %23 = load i64, ptr %arraySize2, align 8
  %24 = alloca ptr, i64 %23, align 8
  store ptr %24, ptr %array2, align 8
  %25 = load ptr, ptr %array2, align 8
  %26 = getelementptr inbounds ptr, ptr %25, i32 0
  store ptr @3, ptr %26, align 8
  %27 = load ptr, ptr %array2, align 8
  %28 = getelementptr inbounds ptr, ptr %27, i32 1
  store ptr @4, ptr %28, align 8
  store i32 0, ptr %idx3, align 4
  store ptr @5, ptr %item4, align 8
  %29 = load ptr, ptr %array2, align 8
  %30 = trunc i64 %23 to i32
  %31 = load i32, ptr %idx3, align 4
  %32 = load ptr, ptr %array2, align 8
  %33 = getelementptr inbounds ptr, ptr %32, i32 %31
  %34 = load ptr, ptr %33, align 8
  store ptr %34, ptr %item4, align 8
  br label %foreach.loop.l15

foreach.loop.l15:                                 ; preds = %foreach.cond.l15, %foreach.end.l6
  %35 = load ptr, ptr %item4, align 8
  %36 = call i32 (ptr, ...) @printf(ptr @6, ptr %35)
  br label %foreach.inc.l15

foreach.inc.l15:                                  ; preds = %foreach.loop.l15
  %idx5 = load i32, ptr %idx3, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, ptr %idx3, align 4
  %37 = getelementptr inbounds ptr, ptr %32, i32 %idx.inc6
  %38 = load ptr, ptr %37, align 8
  store ptr %38, ptr %item4, align 8
  br label %foreach.cond.l15

foreach.cond.l15:                                 ; preds = %foreach.inc.l15
  %39 = load i32, ptr %idx3, align 4
  %40 = icmp ult i32 %39, %30
  br i1 %40, label %foreach.loop.l15, label %foreach.end.l15

foreach.end.l15:                                  ; preds = %foreach.cond.l15
  %41 = call i32 (ptr, ...) @printf(ptr @7)
  store i32 3, ptr %arraySize3, align 4
  %42 = load i32, ptr %arraySize3, align 4
  %43 = load i32, ptr %arraySize3, align 4
  %44 = load i32, ptr %arraySize3, align 4
  %45 = alloca i32, i32 %42, align 4
  %46 = getelementptr inbounds i32, ptr %45, i32 0
  store i32 1, ptr %46, align 4
  %47 = getelementptr inbounds i32, ptr %45, i32 1
  store i32 %43, ptr %47, align 4
  %48 = getelementptr inbounds i32, ptr %45, i32 2
  store i32 %44, ptr %48, align 4
  store ptr %45, ptr %array3, align 8
  %49 = load ptr, ptr %array3, align 8
  store ptr %49, ptr %array37, align 8
  store i32 0, ptr %idx8, align 4
  store i32 0, ptr %item9, align 4
  %50 = load ptr, ptr %array37, align 8
  %51 = load i32, ptr %idx8, align 4
  %52 = load ptr, ptr %array37, align 8
  %53 = getelementptr inbounds i32, ptr %52, i32 %51
  %54 = load i32, ptr %53, align 4
  store i32 %54, ptr %item9, align 4
  br label %foreach.loop.l22

foreach.loop.l22:                                 ; preds = %foreach.cond.l22, %foreach.end.l15
  %55 = load i32, ptr %item9, align 4
  %56 = call i32 (ptr, ...) @printf(ptr @8, i32 %55)
  br label %foreach.inc.l22

foreach.inc.l22:                                  ; preds = %foreach.loop.l22
  %idx10 = load i32, ptr %idx8, align 4
  %idx.inc11 = add i32 %idx10, 1
  store i32 %idx.inc11, ptr %idx8, align 4
  %57 = getelementptr inbounds i32, ptr %52, i32 %idx.inc11
  %58 = load i32, ptr %57, align 4
  store i32 %58, ptr %item9, align 4
  br label %foreach.cond.l22

foreach.cond.l22:                                 ; preds = %foreach.inc.l22
  %59 = load i32, ptr %idx8, align 4
  %60 = icmp ult i32 %59, %42
  br i1 %60, label %foreach.loop.l22, label %foreach.end.l22

foreach.end.l22:                                  ; preds = %foreach.cond.l22
  %61 = call i32 (ptr, ...) @printf(ptr @9)
  store i32 3, ptr %arraySize4, align 4
  %62 = load i32, ptr %arraySize4, align 4
  %63 = alloca i32, i32 %62, align 4
  store ptr %63, ptr %array4, align 8
  %64 = load i32, ptr %arraySize4, align 4
  %65 = load i32, ptr %arraySize4, align 4
  %66 = alloca i32, i32 %62, align 4
  %67 = getelementptr inbounds i32, ptr %66, i32 0
  store i32 1, ptr %67, align 4
  %68 = getelementptr inbounds i32, ptr %66, i32 1
  store i32 %64, ptr %68, align 4
  %69 = getelementptr inbounds i32, ptr %66, i32 2
  store i32 %65, ptr %69, align 4
  store ptr %66, ptr %array412, align 8
  %70 = load ptr, ptr %array412, align 8
  store ptr %70, ptr %array4, align 8
  store i32 0, ptr %idx13, align 4
  store i32 0, ptr %item14, align 4
  %71 = load ptr, ptr %array4, align 8
  %72 = load i32, ptr %idx13, align 4
  %73 = load ptr, ptr %array4, align 8
  %74 = getelementptr inbounds i32, ptr %73, i32 %72
  %75 = load i32, ptr %74, align 4
  store i32 %75, ptr %item14, align 4
  br label %foreach.loop.l30

foreach.loop.l30:                                 ; preds = %foreach.cond.l30, %foreach.end.l22
  %76 = load i32, ptr %item14, align 4
  %77 = call i32 (ptr, ...) @printf(ptr @10, i32 %76)
  br label %foreach.inc.l30

foreach.inc.l30:                                  ; preds = %foreach.loop.l30
  %idx15 = load i32, ptr %idx13, align 4
  %idx.inc16 = add i32 %idx15, 1
  store i32 %idx.inc16, ptr %idx13, align 4
  %78 = getelementptr inbounds i32, ptr %73, i32 %idx.inc16
  %79 = load i32, ptr %78, align 4
  store i32 %79, ptr %item14, align 4
  br label %foreach.cond.l30

foreach.cond.l30:                                 ; preds = %foreach.inc.l30
  %80 = load i32, ptr %idx13, align 4
  %81 = icmp ult i32 %80, %62
  br i1 %81, label %foreach.loop.l30, label %foreach.end.l30

foreach.end.l30:                                  ; preds = %foreach.cond.l30
  call void @llvm.stackrestore(ptr %2)
  %82 = load i32, ptr %result, align 4
  ret i32 %82
}

declare i32 @printf(ptr, ...)

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave() #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore(ptr) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn }
