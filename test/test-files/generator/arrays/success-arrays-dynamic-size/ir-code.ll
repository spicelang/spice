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
  %array1 = alloca i32*, align 8
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  %arraySize2 = alloca i64, align 8
  %array2 = alloca i8**, align 8
  %idx2 = alloca i32, align 4
  %item3 = alloca i8*, align 8
  %arraySize3 = alloca i32, align 4
  %array3 = alloca i32*, align 8
  %idx6 = alloca i32, align 4
  %item7 = alloca i32, align 4
  %arraySize4 = alloca i32, align 4
  %array4 = alloca i32*, align 8
  %array410 = alloca i32*, align 8
  %idx11 = alloca i32, align 4
  %item12 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0))
  store i16 4, i16* %arraySize1, align 2
  %1 = load i16, i16* %arraySize1, align 2
  %2 = call i8* @llvm.stacksave()
  %3 = alloca i32, i16 %1, align 4
  %4 = getelementptr inbounds i32, i32* %3, i32 0
  store i32 1, i32* %4, align 4
  %5 = getelementptr inbounds i32, i32* %3, i32 1
  store i32 2, i32* %5, align 4
  %6 = getelementptr inbounds i32, i32* %3, i32 2
  store i32 3, i32* %6, align 4
  store i32* %3, i32** %array1, align 8
  %7 = load i32*, i32** %array1, align 8
  %8 = getelementptr inbounds i32, i32* %7, i32 3
  store i32 0, i32* %8, align 4
  store i32 0, i32* %idx, align 4
  store i32 0, i32* %item, align 4
  %9 = load i32*, i32** %array1, align 8
  %10 = sext i16 %1 to i32
  %11 = load i32, i32* %idx, align 4
  %12 = load i32*, i32** %array1, align 8
  %13 = getelementptr inbounds i32, i32* %12, i32 %11
  %14 = load i32, i32* %13, align 4
  store i32 %14, i32* %item, align 4
  br label %foreach.loop.l6

foreach.loop.l6:                                  ; preds = %foreach.cond.l6, %entry.l1
  %15 = load i32, i32* %item, align 4
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0), i32 %15)
  br label %foreach.inc.l6

foreach.inc.l6:                                   ; preds = %foreach.loop.l6
  %idx1 = load i32, i32* %idx, align 4
  %idx.inc = add i32 %idx1, 1
  store i32 %idx.inc, i32* %idx, align 4
  %17 = getelementptr inbounds i32, i32* %12, i32 %idx.inc
  %18 = load i32, i32* %17, align 4
  store i32 %18, i32* %item, align 4
  br label %foreach.cond.l6

foreach.cond.l6:                                  ; preds = %foreach.inc.l6
  %19 = load i32, i32* %idx, align 4
  %20 = icmp ult i32 %19, %10
  br i1 %20, label %foreach.loop.l6, label %foreach.end.l6

foreach.end.l6:                                   ; preds = %foreach.cond.l6
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @2, i32 0, i32 0))
  store i64 2, i64* %arraySize2, align 8
  %22 = load i64, i64* %arraySize2, align 8
  %23 = call i8* @llvm.stacksave()
  %24 = alloca i8*, i64 %22, align 8
  store i8** %24, i8*** %array2, align 8
  %25 = load i8**, i8*** %array2, align 8
  %26 = getelementptr inbounds i8*, i8** %25, i32 0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @3, i32 0, i32 0), i8** %26, align 8
  %27 = load i8**, i8*** %array2, align 8
  %28 = getelementptr inbounds i8*, i8** %27, i32 1
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @4, i32 0, i32 0), i8** %28, align 8
  store i32 0, i32* %idx2, align 4
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @5, i32 0, i32 0), i8** %item3, align 8
  %29 = load i8**, i8*** %array2, align 8
  %30 = trunc i64 %22 to i32
  %31 = load i32, i32* %idx2, align 4
  %32 = load i8**, i8*** %array2, align 8
  %33 = getelementptr inbounds i8*, i8** %32, i32 %31
  %34 = load i8*, i8** %33, align 8
  store i8* %34, i8** %item3, align 8
  br label %foreach.loop.l15

foreach.loop.l15:                                 ; preds = %foreach.cond.l15, %foreach.end.l6
  %35 = load i8*, i8** %item3, align 8
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @6, i32 0, i32 0), i8* %35)
  br label %foreach.inc.l15

foreach.inc.l15:                                  ; preds = %foreach.loop.l15
  %idx4 = load i32, i32* %idx2, align 4
  %idx.inc5 = add i32 %idx4, 1
  store i32 %idx.inc5, i32* %idx2, align 4
  %37 = getelementptr inbounds i8*, i8** %32, i32 %idx.inc5
  %38 = load i8*, i8** %37, align 8
  store i8* %38, i8** %item3, align 8
  br label %foreach.cond.l15

foreach.cond.l15:                                 ; preds = %foreach.inc.l15
  %39 = load i32, i32* %idx2, align 4
  %40 = icmp ult i32 %39, %30
  br i1 %40, label %foreach.loop.l15, label %foreach.end.l15

foreach.end.l15:                                  ; preds = %foreach.cond.l15
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @7, i32 0, i32 0))
  store i32 3, i32* %arraySize3, align 4
  %42 = load i32, i32* %arraySize3, align 4
  %43 = load i32, i32* %arraySize3, align 4
  %44 = load i32, i32* %arraySize3, align 4
  %45 = call i8* @llvm.stacksave()
  %46 = alloca i32, i32 %42, align 4
  %47 = getelementptr inbounds i32, i32* %46, i32 0
  store i32 1, i32* %47, align 4
  %48 = getelementptr inbounds i32, i32* %46, i32 1
  store i32 %43, i32* %48, align 4
  %49 = getelementptr inbounds i32, i32* %46, i32 2
  store i32 %44, i32* %49, align 4
  store i32* %46, i32** %array3, align 8
  store i32 0, i32* %idx6, align 4
  store i32 0, i32* %item7, align 4
  %50 = load i32*, i32** %array3, align 8
  %51 = load i32, i32* %idx6, align 4
  %52 = load i32*, i32** %array3, align 8
  %53 = getelementptr inbounds i32, i32* %52, i32 %51
  %54 = load i32, i32* %53, align 4
  store i32 %54, i32* %item7, align 4
  br label %foreach.loop.l22

foreach.loop.l22:                                 ; preds = %foreach.cond.l22, %foreach.end.l15
  %55 = load i32, i32* %item7, align 4
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @8, i32 0, i32 0), i32 %55)
  br label %foreach.inc.l22

foreach.inc.l22:                                  ; preds = %foreach.loop.l22
  %idx8 = load i32, i32* %idx6, align 4
  %idx.inc9 = add i32 %idx8, 1
  store i32 %idx.inc9, i32* %idx6, align 4
  %57 = getelementptr inbounds i32, i32* %52, i32 %idx.inc9
  %58 = load i32, i32* %57, align 4
  store i32 %58, i32* %item7, align 4
  br label %foreach.cond.l22

foreach.cond.l22:                                 ; preds = %foreach.inc.l22
  %59 = load i32, i32* %idx6, align 4
  %60 = icmp ult i32 %59, %42
  br i1 %60, label %foreach.loop.l22, label %foreach.end.l22

foreach.end.l22:                                  ; preds = %foreach.cond.l22
  %61 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @9, i32 0, i32 0))
  store i32 3, i32* %arraySize4, align 4
  %62 = load i32, i32* %arraySize4, align 4
  %63 = call i8* @llvm.stacksave()
  %64 = alloca i32, i32 %62, align 4
  store i32* %64, i32** %array4, align 8
  %65 = load i32, i32* %arraySize4, align 4
  %66 = load i32, i32* %arraySize4, align 4
  %67 = call i8* @llvm.stacksave()
  %68 = alloca i32, align 4
  %69 = getelementptr inbounds i32, i32* %68, i32 0
  store i32 1, i32* %69, align 4
  %70 = getelementptr inbounds i32, i32* %68, i32 1
  store i32 %65, i32* %70, align 4
  %71 = getelementptr inbounds i32, i32* %68, i32 2
  store i32 %66, i32* %71, align 4
  store i32* %68, i32** %array410, align 8
  %72 = load i32*, i32** %array410, align 8
  store i32* %72, i32** %array4, align 8
  store i32 0, i32* %idx11, align 4
  store i32 0, i32* %item12, align 4
  %73 = load i32*, i32** %array4, align 8
  %74 = load i32, i32* %idx11, align 4
  %75 = load i32*, i32** %array4, align 8
  %76 = getelementptr inbounds i32, i32* %75, i32 %74
  %77 = load i32, i32* %76, align 4
  store i32 %77, i32* %item12, align 4
  br label %foreach.loop.l30

foreach.loop.l30:                                 ; preds = %foreach.cond.l30, %foreach.end.l22
  %78 = load i32, i32* %item12, align 4
  %79 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @10, i32 0, i32 0), i32 %78)
  br label %foreach.inc.l30

foreach.inc.l30:                                  ; preds = %foreach.loop.l30
  %idx13 = load i32, i32* %idx11, align 4
  %idx.inc14 = add i32 %idx13, 1
  store i32 %idx.inc14, i32* %idx11, align 4
  %80 = getelementptr inbounds i32, i32* %75, i32 %idx.inc14
  %81 = load i32, i32* %80, align 4
  store i32 %81, i32* %item12, align 4
  br label %foreach.cond.l30

foreach.cond.l30:                                 ; preds = %foreach.inc.l30
  %82 = load i32, i32* %idx11, align 4
  %83 = icmp ult i32 %82, %62
  br i1 %83, label %foreach.loop.l30, label %foreach.end.l30

foreach.end.l30:                                  ; preds = %foreach.cond.l30
  call void @llvm.stackrestore(i8* %67)
  %84 = load i32, i32* %result, align 4
  ret i32 %84
}

declare i32 @printf(i8*, ...)

; Function Attrs: nofree nosync nounwind willreturn
declare i8* @llvm.stacksave() #0

; Function Attrs: nofree nosync nounwind willreturn
declare void @llvm.stackrestore(i8*) #0

attributes #0 = { nofree nosync nounwind willreturn }
