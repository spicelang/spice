; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@MAX_VALUE = external local_unnamed_addr global i32
@printf.str.1 = private unnamed_addr constant [10 x i8] c"%d \09\09 %d\0A\00", align 1
@str = private unnamed_addr constant [31 x i8] c"Vertex \09\09 Distance from source\00", align 1
@str.1 = private unnamed_addr constant [55 x i8] c"Computing shortest paths with Dijkstra's algorithm ...\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"Done.\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define private fastcc void @_p__void__void__dijkstra__intarrayarray_int(ptr nocapture readonly %0) unnamed_addr #0 {
for.body.L31:
  %dist = alloca [9 x i32], align 4
  %sptSet = alloca [9 x i1], align 1
  %.fca.1.gep19 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 1
  %.fca.2.gep20 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 2
  %.fca.3.gep21 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 3
  %.fca.4.gep22 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 4
  %.fca.5.gep23 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 5
  %.fca.6.gep24 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 6
  %.fca.7.gep25 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 7
  %.fca.8.gep26 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 8
  %.fca.1.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 1
  %.fca.2.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 2
  %.fca.3.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 3
  %.fca.4.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 4
  %.fca.5.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 5
  %.fca.6.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 6
  %.fca.7.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 7
  %.fca.8.gep = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 8
  %1 = load i32, ptr @MAX_VALUE, align 4
  store i1 false, ptr %sptSet, align 1
  store i32 %1, ptr %.fca.1.gep19, align 4
  store i1 false, ptr %.fca.1.gep, align 1
  store i32 %1, ptr %.fca.2.gep20, align 4
  store i1 false, ptr %.fca.2.gep, align 1
  store i32 %1, ptr %.fca.3.gep21, align 4
  store i1 false, ptr %.fca.3.gep, align 1
  store i32 %1, ptr %.fca.4.gep22, align 4
  store i1 false, ptr %.fca.4.gep, align 1
  store i32 %1, ptr %.fca.5.gep23, align 4
  store i1 false, ptr %.fca.5.gep, align 1
  store i32 %1, ptr %.fca.6.gep24, align 4
  store i1 false, ptr %.fca.6.gep, align 1
  store i32 %1, ptr %.fca.7.gep25, align 4
  store i1 false, ptr %.fca.7.gep, align 1
  store i32 %1, ptr %.fca.8.gep26, align 4
  store i1 false, ptr %.fca.8.gep, align 1
  store i32 0, ptr %dist, align 4
  br label %for.body.L39

for.body.L39:                                     ; preds = %for.body.L31, %for.tail.L42.8
  %2 = phi i32 [ %1, %for.body.L31 ], [ %107, %for.tail.L42.8 ]
  %3 = phi i1 [ false, %for.body.L31 ], [ %101, %for.tail.L42.8 ]
  %4 = phi i32 [ %1, %for.body.L31 ], [ %100, %for.tail.L42.8 ]
  %5 = phi i1 [ false, %for.body.L31 ], [ %94, %for.tail.L42.8 ]
  %6 = phi i32 [ %1, %for.body.L31 ], [ %93, %for.tail.L42.8 ]
  %7 = phi i1 [ false, %for.body.L31 ], [ %87, %for.tail.L42.8 ]
  %8 = phi i32 [ %1, %for.body.L31 ], [ %86, %for.tail.L42.8 ]
  %9 = phi i1 [ false, %for.body.L31 ], [ %80, %for.tail.L42.8 ]
  %10 = phi i32 [ %1, %for.body.L31 ], [ %79, %for.tail.L42.8 ]
  %11 = phi i1 [ false, %for.body.L31 ], [ %73, %for.tail.L42.8 ]
  %12 = phi i32 [ %1, %for.body.L31 ], [ %72, %for.tail.L42.8 ]
  %13 = phi i1 [ false, %for.body.L31 ], [ %66, %for.tail.L42.8 ]
  %14 = phi i32 [ %1, %for.body.L31 ], [ %65, %for.tail.L42.8 ]
  %15 = phi i1 [ false, %for.body.L31 ], [ %59, %for.tail.L42.8 ]
  %16 = phi i32 [ %1, %for.body.L31 ], [ %58, %for.tail.L42.8 ]
  %17 = phi i1 [ false, %for.body.L31 ], [ %52, %for.tail.L42.8 ]
  %18 = phi i32 [ 0, %for.body.L31 ], [ %51, %for.tail.L42.8 ]
  %19 = phi i1 [ false, %for.body.L31 ], [ %45, %for.tail.L42.8 ]
  %count.04 = phi i32 [ 0, %for.body.L31 ], [ %108, %for.tail.L42.8 ]
  %20 = tail call i32 @llvm.smin.i32(i32 %18, i32 %1)
  %min.1.i = select i1 %19, i32 %1, i32 %20
  %.not.1.i = icmp sle i32 %16, %min.1.i
  %21 = tail call i32 @llvm.smin.i32(i32 %16, i32 %min.1.i)
  %not. = xor i1 %17, true
  %narrow = select i1 %not., i1 %.not.1.i, i1 false
  %min.1.1.i = select i1 %17, i32 %min.1.i, i32 %21
  %.not.2.i = icmp sgt i32 %14, %min.1.1.i
  %22 = tail call i32 @llvm.smin.i32(i32 %14, i32 %min.1.1.i)
  %23 = select i1 %15, i1 true, i1 %.not.2.i
  %min.1.2.i = select i1 %15, i32 %min.1.1.i, i32 %22
  %.not.3.i = icmp sgt i32 %12, %min.1.2.i
  %24 = tail call i32 @llvm.smin.i32(i32 %12, i32 %min.1.2.i)
  %25 = select i1 %13, i1 true, i1 %.not.3.i
  %min.1.3.i = select i1 %13, i32 %min.1.2.i, i32 %24
  %.not.4.i = icmp sgt i32 %10, %min.1.3.i
  %26 = tail call i32 @llvm.smin.i32(i32 %10, i32 %min.1.3.i)
  %27 = select i1 %11, i1 true, i1 %.not.4.i
  %min.1.4.i = select i1 %11, i32 %min.1.3.i, i32 %26
  %.not.5.i = icmp sgt i32 %8, %min.1.4.i
  %28 = tail call i32 @llvm.smin.i32(i32 %8, i32 %min.1.4.i)
  %29 = select i1 %9, i1 true, i1 %.not.5.i
  %min.1.5.i = select i1 %9, i32 %min.1.4.i, i32 %28
  %.not.6.i = icmp sgt i32 %6, %min.1.5.i
  %30 = tail call i32 @llvm.smin.i32(i32 %6, i32 %min.1.5.i)
  %31 = select i1 %7, i1 true, i1 %.not.6.i
  %min.1.6.i = select i1 %7, i32 %min.1.5.i, i32 %30
  %.not.7.i = icmp sgt i32 %4, %min.1.6.i
  %32 = tail call i32 @llvm.smin.i32(i32 %4, i32 %min.1.6.i)
  %33 = select i1 %5, i1 true, i1 %.not.7.i
  %min.1.7.i = select i1 %5, i32 %min.1.6.i, i32 %32
  %.not.8.i = icmp sgt i32 %2, %min.1.7.i
  %34 = select i1 %3, i1 true, i1 %.not.8.i
  %35 = zext i1 %narrow to i64
  %36 = select i1 %23, i64 %35, i64 2
  %37 = select i1 %25, i64 %36, i64 3
  %38 = select i1 %27, i64 %37, i64 4
  %39 = select i1 %29, i64 %38, i64 5
  %40 = select i1 %31, i64 %39, i64 6
  %41 = select i1 %33, i64 %40, i64 7
  %42 = select i1 %34, i64 %41, i64 8
  %43 = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 %42
  store i1 true, ptr %43, align 1
  %44 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 %42
  %45 = load i1, ptr %sptSet, align 1
  br i1 %45, label %for.tail.L42, label %land.1.L43C17

land.1.L43C17:                                    ; preds = %for.body.L39
  %46 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 0
  %47 = load i32, ptr %46, align 4
  %.not = icmp eq i32 %47, 0
  br i1 %.not, label %for.tail.L42, label %land.2.L43C17

land.2.L43C17:                                    ; preds = %land.1.L43C17
  %48 = load i32, ptr %44, align 4
  %.not1 = icmp eq i32 %48, %1
  br i1 %.not1, label %for.tail.L42, label %land.3.L43C17

land.3.L43C17:                                    ; preds = %land.2.L43C17
  %49 = add i32 %48, %47
  %50 = tail call i32 @llvm.smin.i32(i32 %49, i32 %18)
  store i32 %50, ptr %dist, align 4
  br label %for.tail.L42

for.tail.L42:                                     ; preds = %land.3.L43C17, %for.body.L39, %land.1.L43C17, %land.2.L43C17
  %51 = phi i32 [ %50, %land.3.L43C17 ], [ %18, %for.body.L39 ], [ %18, %land.1.L43C17 ], [ %18, %land.2.L43C17 ]
  %52 = load i1, ptr %.fca.1.gep, align 1
  br i1 %52, label %for.tail.L42.1, label %land.1.L43C17.1

land.1.L43C17.1:                                  ; preds = %for.tail.L42
  %53 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 1
  %54 = load i32, ptr %53, align 4
  %.not.1 = icmp eq i32 %54, 0
  br i1 %.not.1, label %for.tail.L42.1, label %land.2.L43C17.1

land.2.L43C17.1:                                  ; preds = %land.1.L43C17.1
  %55 = load i32, ptr %44, align 4
  %.not1.1 = icmp eq i32 %55, %1
  br i1 %.not1.1, label %for.tail.L42.1, label %land.3.L43C17.1

land.3.L43C17.1:                                  ; preds = %land.2.L43C17.1
  %56 = add i32 %55, %54
  %57 = tail call i32 @llvm.smin.i32(i32 %56, i32 %16)
  store i32 %57, ptr %.fca.1.gep19, align 4
  br label %for.tail.L42.1

for.tail.L42.1:                                   ; preds = %land.3.L43C17.1, %land.2.L43C17.1, %land.1.L43C17.1, %for.tail.L42
  %58 = phi i32 [ %57, %land.3.L43C17.1 ], [ %16, %land.2.L43C17.1 ], [ %16, %land.1.L43C17.1 ], [ %16, %for.tail.L42 ]
  %59 = load i1, ptr %.fca.2.gep, align 1
  br i1 %59, label %for.tail.L42.2, label %land.1.L43C17.2

land.1.L43C17.2:                                  ; preds = %for.tail.L42.1
  %60 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 2
  %61 = load i32, ptr %60, align 4
  %.not.2 = icmp eq i32 %61, 0
  br i1 %.not.2, label %for.tail.L42.2, label %land.2.L43C17.2

land.2.L43C17.2:                                  ; preds = %land.1.L43C17.2
  %62 = load i32, ptr %44, align 4
  %.not1.2 = icmp eq i32 %62, %1
  br i1 %.not1.2, label %for.tail.L42.2, label %land.3.L43C17.2

land.3.L43C17.2:                                  ; preds = %land.2.L43C17.2
  %63 = add i32 %62, %61
  %64 = tail call i32 @llvm.smin.i32(i32 %63, i32 %14)
  store i32 %64, ptr %.fca.2.gep20, align 4
  br label %for.tail.L42.2

for.tail.L42.2:                                   ; preds = %land.3.L43C17.2, %land.2.L43C17.2, %land.1.L43C17.2, %for.tail.L42.1
  %65 = phi i32 [ %64, %land.3.L43C17.2 ], [ %14, %land.2.L43C17.2 ], [ %14, %land.1.L43C17.2 ], [ %14, %for.tail.L42.1 ]
  %66 = load i1, ptr %.fca.3.gep, align 1
  br i1 %66, label %for.tail.L42.3, label %land.1.L43C17.3

land.1.L43C17.3:                                  ; preds = %for.tail.L42.2
  %67 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 3
  %68 = load i32, ptr %67, align 4
  %.not.3 = icmp eq i32 %68, 0
  br i1 %.not.3, label %for.tail.L42.3, label %land.2.L43C17.3

land.2.L43C17.3:                                  ; preds = %land.1.L43C17.3
  %69 = load i32, ptr %44, align 4
  %.not1.3 = icmp eq i32 %69, %1
  br i1 %.not1.3, label %for.tail.L42.3, label %land.3.L43C17.3

land.3.L43C17.3:                                  ; preds = %land.2.L43C17.3
  %70 = add i32 %69, %68
  %71 = tail call i32 @llvm.smin.i32(i32 %70, i32 %12)
  store i32 %71, ptr %.fca.3.gep21, align 4
  br label %for.tail.L42.3

for.tail.L42.3:                                   ; preds = %land.3.L43C17.3, %land.2.L43C17.3, %land.1.L43C17.3, %for.tail.L42.2
  %72 = phi i32 [ %71, %land.3.L43C17.3 ], [ %12, %land.2.L43C17.3 ], [ %12, %land.1.L43C17.3 ], [ %12, %for.tail.L42.2 ]
  %73 = load i1, ptr %.fca.4.gep, align 1
  br i1 %73, label %for.tail.L42.4, label %land.1.L43C17.4

land.1.L43C17.4:                                  ; preds = %for.tail.L42.3
  %74 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 4
  %75 = load i32, ptr %74, align 4
  %.not.4 = icmp eq i32 %75, 0
  br i1 %.not.4, label %for.tail.L42.4, label %land.2.L43C17.4

land.2.L43C17.4:                                  ; preds = %land.1.L43C17.4
  %76 = load i32, ptr %44, align 4
  %.not1.4 = icmp eq i32 %76, %1
  br i1 %.not1.4, label %for.tail.L42.4, label %land.3.L43C17.4

land.3.L43C17.4:                                  ; preds = %land.2.L43C17.4
  %77 = add i32 %76, %75
  %78 = tail call i32 @llvm.smin.i32(i32 %77, i32 %10)
  store i32 %78, ptr %.fca.4.gep22, align 4
  br label %for.tail.L42.4

for.tail.L42.4:                                   ; preds = %land.3.L43C17.4, %land.2.L43C17.4, %land.1.L43C17.4, %for.tail.L42.3
  %79 = phi i32 [ %78, %land.3.L43C17.4 ], [ %10, %land.2.L43C17.4 ], [ %10, %land.1.L43C17.4 ], [ %10, %for.tail.L42.3 ]
  %80 = load i1, ptr %.fca.5.gep, align 1
  br i1 %80, label %for.tail.L42.5, label %land.1.L43C17.5

land.1.L43C17.5:                                  ; preds = %for.tail.L42.4
  %81 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 5
  %82 = load i32, ptr %81, align 4
  %.not.5 = icmp eq i32 %82, 0
  br i1 %.not.5, label %for.tail.L42.5, label %land.2.L43C17.5

land.2.L43C17.5:                                  ; preds = %land.1.L43C17.5
  %83 = load i32, ptr %44, align 4
  %.not1.5 = icmp eq i32 %83, %1
  br i1 %.not1.5, label %for.tail.L42.5, label %land.3.L43C17.5

land.3.L43C17.5:                                  ; preds = %land.2.L43C17.5
  %84 = add i32 %83, %82
  %85 = tail call i32 @llvm.smin.i32(i32 %84, i32 %8)
  store i32 %85, ptr %.fca.5.gep23, align 4
  br label %for.tail.L42.5

for.tail.L42.5:                                   ; preds = %land.3.L43C17.5, %land.2.L43C17.5, %land.1.L43C17.5, %for.tail.L42.4
  %86 = phi i32 [ %85, %land.3.L43C17.5 ], [ %8, %land.2.L43C17.5 ], [ %8, %land.1.L43C17.5 ], [ %8, %for.tail.L42.4 ]
  %87 = load i1, ptr %.fca.6.gep, align 1
  br i1 %87, label %for.tail.L42.6, label %land.1.L43C17.6

land.1.L43C17.6:                                  ; preds = %for.tail.L42.5
  %88 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 6
  %89 = load i32, ptr %88, align 4
  %.not.6 = icmp eq i32 %89, 0
  br i1 %.not.6, label %for.tail.L42.6, label %land.2.L43C17.6

land.2.L43C17.6:                                  ; preds = %land.1.L43C17.6
  %90 = load i32, ptr %44, align 4
  %.not1.6 = icmp eq i32 %90, %1
  br i1 %.not1.6, label %for.tail.L42.6, label %land.3.L43C17.6

land.3.L43C17.6:                                  ; preds = %land.2.L43C17.6
  %91 = add i32 %90, %89
  %92 = tail call i32 @llvm.smin.i32(i32 %91, i32 %6)
  store i32 %92, ptr %.fca.6.gep24, align 4
  br label %for.tail.L42.6

for.tail.L42.6:                                   ; preds = %land.3.L43C17.6, %land.2.L43C17.6, %land.1.L43C17.6, %for.tail.L42.5
  %93 = phi i32 [ %92, %land.3.L43C17.6 ], [ %6, %land.2.L43C17.6 ], [ %6, %land.1.L43C17.6 ], [ %6, %for.tail.L42.5 ]
  %94 = load i1, ptr %.fca.7.gep, align 1
  br i1 %94, label %for.tail.L42.7, label %land.1.L43C17.7

land.1.L43C17.7:                                  ; preds = %for.tail.L42.6
  %95 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 7
  %96 = load i32, ptr %95, align 4
  %.not.7 = icmp eq i32 %96, 0
  br i1 %.not.7, label %for.tail.L42.7, label %land.2.L43C17.7

land.2.L43C17.7:                                  ; preds = %land.1.L43C17.7
  %97 = load i32, ptr %44, align 4
  %.not1.7 = icmp eq i32 %97, %1
  br i1 %.not1.7, label %for.tail.L42.7, label %land.3.L43C17.7

land.3.L43C17.7:                                  ; preds = %land.2.L43C17.7
  %98 = add i32 %97, %96
  %99 = tail call i32 @llvm.smin.i32(i32 %98, i32 %4)
  store i32 %99, ptr %.fca.7.gep25, align 4
  br label %for.tail.L42.7

for.tail.L42.7:                                   ; preds = %land.3.L43C17.7, %land.2.L43C17.7, %land.1.L43C17.7, %for.tail.L42.6
  %100 = phi i32 [ %99, %land.3.L43C17.7 ], [ %4, %land.2.L43C17.7 ], [ %4, %land.1.L43C17.7 ], [ %4, %for.tail.L42.6 ]
  %101 = load i1, ptr %.fca.8.gep, align 1
  br i1 %101, label %for.tail.L42.8, label %land.1.L43C17.8

land.1.L43C17.8:                                  ; preds = %for.tail.L42.7
  %102 = getelementptr inbounds [9 x i32], ptr %0, i64 %42, i64 8
  %103 = load i32, ptr %102, align 4
  %.not.8 = icmp eq i32 %103, 0
  br i1 %.not.8, label %for.tail.L42.8, label %land.2.L43C17.8

land.2.L43C17.8:                                  ; preds = %land.1.L43C17.8
  %104 = load i32, ptr %44, align 4
  %.not1.8 = icmp eq i32 %104, %1
  br i1 %.not1.8, label %for.tail.L42.8, label %land.3.L43C17.8

land.3.L43C17.8:                                  ; preds = %land.2.L43C17.8
  %105 = add i32 %104, %103
  %106 = tail call i32 @llvm.smin.i32(i32 %105, i32 %2)
  store i32 %106, ptr %.fca.8.gep26, align 4
  br label %for.tail.L42.8

for.tail.L42.8:                                   ; preds = %land.3.L43C17.8, %land.2.L43C17.8, %land.1.L43C17.8, %for.tail.L42.7
  %107 = phi i32 [ %106, %land.3.L43C17.8 ], [ %2, %land.2.L43C17.8 ], [ %2, %land.1.L43C17.8 ], [ %2, %for.tail.L42.7 ]
  %108 = add nuw nsw i32 %count.04, 1
  %exitcond.not = icmp eq i32 %108, 8
  br i1 %exitcond.not, label %for.exit.L39, label %for.body.L39

for.exit.L39:                                     ; preds = %for.tail.L42.8
  %puts.i = tail call i32 @puts(ptr nonnull @str)
  %109 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 0, i32 %51)
  %110 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 1, i32 %58)
  %111 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 2, i32 %65)
  %112 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 3, i32 %72)
  %113 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 4, i32 %79)
  %114 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 5, i32 %86)
  %115 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 6, i32 %93)
  %116 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 7, i32 %100)
  %117 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 8, i32 %107)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %graph = alloca [9 x [9 x i32]], align 4
  store i32 0, ptr %graph, align 4
  %.fca.0.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 0, i64 1
  store i32 4, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 0, i64 2
  %.fca.0.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 0, i64 7
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(20) %.fca.0.2.gep, i8 0, i64 20, i1 false)
  store i32 8, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 0, i64 8
  store i32 0, ptr %.fca.0.8.gep, align 4
  %.fca.1.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 0
  store i32 4, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 1
  store i32 0, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 2
  store i32 8, ptr %.fca.1.2.gep, align 4
  %.fca.1.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 3
  %.fca.1.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 7
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(16) %.fca.1.3.gep, i8 0, i64 16, i1 false)
  store i32 11, ptr %.fca.1.7.gep, align 4
  %.fca.1.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 1, i64 8
  store i32 0, ptr %.fca.1.8.gep, align 4
  %.fca.2.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 0
  store i32 0, ptr %.fca.2.0.gep, align 4
  %.fca.2.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 1
  store i32 8, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 2
  store i32 0, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 3
  store i32 7, ptr %.fca.2.3.gep, align 4
  %.fca.2.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 4
  store i32 0, ptr %.fca.2.4.gep, align 4
  %.fca.2.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 5
  store i32 4, ptr %.fca.2.5.gep, align 4
  %.fca.2.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 6
  store i32 0, ptr %.fca.2.6.gep, align 4
  %.fca.2.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 7
  store i32 0, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 2, i64 8
  store i32 2, ptr %.fca.2.8.gep, align 4
  %.fca.3.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 0
  store i32 0, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 1
  store i32 0, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 2
  store i32 7, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 3
  store i32 0, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 4
  store i32 9, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 5
  store i32 14, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 3, i64 6
  %.fca.4.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 4, i64 3
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(24) %.fca.3.6.gep, i8 0, i64 24, i1 false)
  store i32 9, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 4, i64 4
  store i32 0, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 4, i64 5
  store i32 10, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 4, i64 6
  %.fca.5.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 2
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(20) %.fca.4.6.gep, i8 0, i64 20, i1 false)
  store i32 4, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 3
  store i32 14, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 4
  store i32 10, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 5
  store i32 0, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 6
  store i32 2, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 5, i64 7
  %.fca.6.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 6, i64 5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(28) %.fca.5.7.gep, i8 0, i64 28, i1 false)
  store i32 2, ptr %.fca.6.5.gep, align 4
  %.fca.6.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 6, i64 6
  store i32 0, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 6, i64 7
  store i32 1, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 6, i64 8
  store i32 6, ptr %.fca.6.8.gep, align 4
  %.fca.7.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 0
  store i32 8, ptr %.fca.7.0.gep, align 4
  %.fca.7.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 1
  store i32 11, ptr %.fca.7.1.gep, align 4
  %.fca.7.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 2
  %.fca.7.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 6
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(16) %.fca.7.2.gep, i8 0, i64 16, i1 false)
  store i32 1, ptr %.fca.7.6.gep, align 4
  %.fca.7.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 7
  store i32 0, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 7, i64 8
  store i32 7, ptr %.fca.7.8.gep, align 4
  %.fca.8.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 0
  store i32 0, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 1
  store i32 0, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 2
  store i32 2, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 3
  store i32 0, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 4
  store i32 0, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 5
  store i32 0, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 6
  store i32 6, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 7
  store i32 7, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %graph, i64 0, i64 8, i64 8
  store i32 0, ptr %.fca.8.8.gep, align 4
  %puts = tail call i32 @puts(ptr nonnull @str.1)
  call fastcc void @_p__void__void__dijkstra__intarrayarray_int(ptr nonnull %graph) #4
  %puts1 = tail call i32 @puts(ptr nonnull @str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare i32 @llvm.smin.i32(i32, i32) #2

; Function Attrs: argmemonly nocallback nofree nounwind willreturn writeonly
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nocallback nofree nosync nounwind readnone speculatable willreturn }
attributes #3 = { argmemonly nocallback nofree nounwind willreturn writeonly }
attributes #4 = { nounwind }
