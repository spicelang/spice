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

for.body.L39:                                     ; preds = %for.tail.L42.8.1, %for.body.L31
  %2 = phi i32 [ %1, %for.body.L31 ], [ %137, %for.tail.L42.8.1 ]
  %3 = phi i1 [ false, %for.body.L31 ], [ %132, %for.tail.L42.8.1 ]
  %4 = phi i32 [ %1, %for.body.L31 ], [ %131, %for.tail.L42.8.1 ]
  %5 = phi i1 [ false, %for.body.L31 ], [ %126, %for.tail.L42.8.1 ]
  %6 = phi i32 [ %1, %for.body.L31 ], [ %125, %for.tail.L42.8.1 ]
  %7 = phi i1 [ false, %for.body.L31 ], [ %120, %for.tail.L42.8.1 ]
  %8 = phi i32 [ %1, %for.body.L31 ], [ %119, %for.tail.L42.8.1 ]
  %9 = phi i1 [ false, %for.body.L31 ], [ %114, %for.tail.L42.8.1 ]
  %10 = phi i32 [ %1, %for.body.L31 ], [ %113, %for.tail.L42.8.1 ]
  %11 = phi i1 [ false, %for.body.L31 ], [ %108, %for.tail.L42.8.1 ]
  %12 = phi i32 [ %1, %for.body.L31 ], [ %107, %for.tail.L42.8.1 ]
  %13 = phi i1 [ false, %for.body.L31 ], [ %102, %for.tail.L42.8.1 ]
  %14 = phi i32 [ %1, %for.body.L31 ], [ %101, %for.tail.L42.8.1 ]
  %15 = phi i1 [ false, %for.body.L31 ], [ %96, %for.tail.L42.8.1 ]
  %16 = phi i32 [ %1, %for.body.L31 ], [ %95, %for.tail.L42.8.1 ]
  %17 = phi i1 [ false, %for.body.L31 ], [ %90, %for.tail.L42.8.1 ]
  %18 = phi i32 [ 0, %for.body.L31 ], [ %89, %for.tail.L42.8.1 ]
  %19 = phi i1 [ false, %for.body.L31 ], [ %84, %for.tail.L42.8.1 ]
  %count.036 = phi i32 [ 0, %for.body.L31 ], [ %138, %for.tail.L42.8.1 ]
  %spec.select7.i = tail call i32 @llvm.smin.i32(i32 %18, i32 %1)
  %min.1.i = select i1 %19, i32 %1, i32 %spec.select7.i
  br i1 %17, label %for.tail.L9.1.i, label %land.1.L10C12.1.i

land.1.L10C12.1.i:                                ; preds = %for.body.L39
  %.not.1.i = icmp sle i32 %16, %min.1.i
  %spec.select.1.i = zext i1 %.not.1.i to i32
  %spec.select7.1.i = tail call i32 @llvm.smin.i32(i32 %16, i32 %min.1.i)
  br label %for.tail.L9.1.i

for.tail.L9.1.i:                                  ; preds = %land.1.L10C12.1.i, %for.body.L39
  %minIndex.1.1.i = phi i32 [ 0, %for.body.L39 ], [ %spec.select.1.i, %land.1.L10C12.1.i ]
  %min.1.1.i = phi i32 [ %min.1.i, %for.body.L39 ], [ %spec.select7.1.i, %land.1.L10C12.1.i ]
  br i1 %15, label %for.tail.L9.2.i, label %land.1.L10C12.2.i

land.1.L10C12.2.i:                                ; preds = %for.tail.L9.1.i
  %.not.2.i = icmp sgt i32 %14, %min.1.1.i
  %spec.select.2.i = select i1 %.not.2.i, i32 %minIndex.1.1.i, i32 2
  %spec.select7.2.i = tail call i32 @llvm.smin.i32(i32 %14, i32 %min.1.1.i)
  br label %for.tail.L9.2.i

for.tail.L9.2.i:                                  ; preds = %land.1.L10C12.2.i, %for.tail.L9.1.i
  %minIndex.1.2.i = phi i32 [ %minIndex.1.1.i, %for.tail.L9.1.i ], [ %spec.select.2.i, %land.1.L10C12.2.i ]
  %min.1.2.i = phi i32 [ %min.1.1.i, %for.tail.L9.1.i ], [ %spec.select7.2.i, %land.1.L10C12.2.i ]
  br i1 %13, label %for.tail.L9.3.i, label %land.1.L10C12.3.i

land.1.L10C12.3.i:                                ; preds = %for.tail.L9.2.i
  %.not.3.i = icmp sgt i32 %12, %min.1.2.i
  %spec.select.3.i = select i1 %.not.3.i, i32 %minIndex.1.2.i, i32 3
  %spec.select7.3.i = tail call i32 @llvm.smin.i32(i32 %12, i32 %min.1.2.i)
  br label %for.tail.L9.3.i

for.tail.L9.3.i:                                  ; preds = %land.1.L10C12.3.i, %for.tail.L9.2.i
  %minIndex.1.3.i = phi i32 [ %minIndex.1.2.i, %for.tail.L9.2.i ], [ %spec.select.3.i, %land.1.L10C12.3.i ]
  %min.1.3.i = phi i32 [ %min.1.2.i, %for.tail.L9.2.i ], [ %spec.select7.3.i, %land.1.L10C12.3.i ]
  br i1 %11, label %for.tail.L9.4.i, label %land.1.L10C12.4.i

land.1.L10C12.4.i:                                ; preds = %for.tail.L9.3.i
  %.not.4.i = icmp sgt i32 %10, %min.1.3.i
  %spec.select.4.i = select i1 %.not.4.i, i32 %minIndex.1.3.i, i32 4
  %spec.select7.4.i = tail call i32 @llvm.smin.i32(i32 %10, i32 %min.1.3.i)
  br label %for.tail.L9.4.i

for.tail.L9.4.i:                                  ; preds = %land.1.L10C12.4.i, %for.tail.L9.3.i
  %minIndex.1.4.i = phi i32 [ %minIndex.1.3.i, %for.tail.L9.3.i ], [ %spec.select.4.i, %land.1.L10C12.4.i ]
  %min.1.4.i = phi i32 [ %min.1.3.i, %for.tail.L9.3.i ], [ %spec.select7.4.i, %land.1.L10C12.4.i ]
  br i1 %9, label %for.tail.L9.5.i, label %land.1.L10C12.5.i

land.1.L10C12.5.i:                                ; preds = %for.tail.L9.4.i
  %.not.5.i = icmp sgt i32 %8, %min.1.4.i
  %spec.select.5.i = select i1 %.not.5.i, i32 %minIndex.1.4.i, i32 5
  %spec.select7.5.i = tail call i32 @llvm.smin.i32(i32 %8, i32 %min.1.4.i)
  br label %for.tail.L9.5.i

for.tail.L9.5.i:                                  ; preds = %land.1.L10C12.5.i, %for.tail.L9.4.i
  %minIndex.1.5.i = phi i32 [ %minIndex.1.4.i, %for.tail.L9.4.i ], [ %spec.select.5.i, %land.1.L10C12.5.i ]
  %min.1.5.i = phi i32 [ %min.1.4.i, %for.tail.L9.4.i ], [ %spec.select7.5.i, %land.1.L10C12.5.i ]
  br i1 %7, label %for.tail.L9.6.i, label %land.1.L10C12.6.i

land.1.L10C12.6.i:                                ; preds = %for.tail.L9.5.i
  %.not.6.i = icmp sgt i32 %6, %min.1.5.i
  %spec.select.6.i = select i1 %.not.6.i, i32 %minIndex.1.5.i, i32 6
  %spec.select7.6.i = tail call i32 @llvm.smin.i32(i32 %6, i32 %min.1.5.i)
  br label %for.tail.L9.6.i

for.tail.L9.6.i:                                  ; preds = %land.1.L10C12.6.i, %for.tail.L9.5.i
  %minIndex.1.6.i = phi i32 [ %minIndex.1.5.i, %for.tail.L9.5.i ], [ %spec.select.6.i, %land.1.L10C12.6.i ]
  %min.1.6.i = phi i32 [ %min.1.5.i, %for.tail.L9.5.i ], [ %spec.select7.6.i, %land.1.L10C12.6.i ]
  br i1 %5, label %for.tail.L9.7.i, label %land.1.L10C12.7.i

land.1.L10C12.7.i:                                ; preds = %for.tail.L9.6.i
  %.not.7.i = icmp sgt i32 %4, %min.1.6.i
  %spec.select.7.i = select i1 %.not.7.i, i32 %minIndex.1.6.i, i32 7
  %spec.select7.7.i = tail call i32 @llvm.smin.i32(i32 %4, i32 %min.1.6.i)
  br label %for.tail.L9.7.i

for.tail.L9.7.i:                                  ; preds = %land.1.L10C12.7.i, %for.tail.L9.6.i
  %minIndex.1.7.i = phi i32 [ %minIndex.1.6.i, %for.tail.L9.6.i ], [ %spec.select.7.i, %land.1.L10C12.7.i ]
  %min.1.7.i = phi i32 [ %min.1.6.i, %for.tail.L9.6.i ], [ %spec.select7.7.i, %land.1.L10C12.7.i ]
  %.not.8.i = icmp sgt i32 %2, %min.1.7.i
  %20 = select i1 %3, i1 true, i1 %.not.8.i
  %21 = zext i32 %minIndex.1.7.i to i64
  %22 = select i1 %20, i64 %21, i64 8
  %23 = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 %22
  store i1 true, ptr %23, align 1
  %24 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 %22
  %25 = load i1, ptr %sptSet, align 1
  br i1 %25, label %for.tail.L42, label %land.1.L43C17

land.1.L43C17:                                    ; preds = %for.tail.L9.7.i
  %26 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 0
  %27 = load i32, ptr %26, align 4
  %.not = icmp eq i32 %27, 0
  br i1 %.not, label %for.tail.L42, label %land.2.L43C17

land.2.L43C17:                                    ; preds = %land.1.L43C17
  %28 = load i32, ptr %24, align 4
  %.not33 = icmp eq i32 %28, %1
  br i1 %.not33, label %for.tail.L42, label %land.3.L43C17

land.3.L43C17:                                    ; preds = %land.2.L43C17
  %29 = add i32 %28, %27
  %spec.store.select = tail call i32 @llvm.smin.i32(i32 %29, i32 %18)
  store i32 %spec.store.select, ptr %dist, align 4
  br label %for.tail.L42

for.tail.L42:                                     ; preds = %land.3.L43C17, %for.tail.L9.7.i, %land.1.L43C17, %land.2.L43C17
  %30 = phi i32 [ %spec.store.select, %land.3.L43C17 ], [ %18, %for.tail.L9.7.i ], [ %18, %land.1.L43C17 ], [ %18, %land.2.L43C17 ]
  %31 = load i1, ptr %.fca.1.gep, align 1
  br i1 %31, label %for.tail.L42.1, label %land.1.L43C17.1

land.1.L43C17.1:                                  ; preds = %for.tail.L42
  %32 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 1
  %33 = load i32, ptr %32, align 4
  %.not.1 = icmp eq i32 %33, 0
  br i1 %.not.1, label %for.tail.L42.1, label %land.2.L43C17.1

land.2.L43C17.1:                                  ; preds = %land.1.L43C17.1
  %34 = load i32, ptr %24, align 4
  %.not33.1 = icmp eq i32 %34, %1
  br i1 %.not33.1, label %for.tail.L42.1, label %land.3.L43C17.1

land.3.L43C17.1:                                  ; preds = %land.2.L43C17.1
  %35 = add i32 %34, %33
  %spec.store.select.1 = tail call i32 @llvm.smin.i32(i32 %35, i32 %16)
  store i32 %spec.store.select.1, ptr %.fca.1.gep19, align 4
  br label %for.tail.L42.1

for.tail.L42.1:                                   ; preds = %land.3.L43C17.1, %land.2.L43C17.1, %land.1.L43C17.1, %for.tail.L42
  %36 = phi i32 [ %spec.store.select.1, %land.3.L43C17.1 ], [ %16, %land.2.L43C17.1 ], [ %16, %land.1.L43C17.1 ], [ %16, %for.tail.L42 ]
  %37 = load i1, ptr %.fca.2.gep, align 1
  br i1 %37, label %for.tail.L42.2, label %land.1.L43C17.2

land.1.L43C17.2:                                  ; preds = %for.tail.L42.1
  %38 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 2
  %39 = load i32, ptr %38, align 4
  %.not.2 = icmp eq i32 %39, 0
  br i1 %.not.2, label %for.tail.L42.2, label %land.2.L43C17.2

land.2.L43C17.2:                                  ; preds = %land.1.L43C17.2
  %40 = load i32, ptr %24, align 4
  %.not33.2 = icmp eq i32 %40, %1
  br i1 %.not33.2, label %for.tail.L42.2, label %land.3.L43C17.2

land.3.L43C17.2:                                  ; preds = %land.2.L43C17.2
  %41 = add i32 %40, %39
  %spec.store.select.2 = tail call i32 @llvm.smin.i32(i32 %41, i32 %14)
  store i32 %spec.store.select.2, ptr %.fca.2.gep20, align 4
  br label %for.tail.L42.2

for.tail.L42.2:                                   ; preds = %land.3.L43C17.2, %land.2.L43C17.2, %land.1.L43C17.2, %for.tail.L42.1
  %42 = phi i32 [ %spec.store.select.2, %land.3.L43C17.2 ], [ %14, %land.2.L43C17.2 ], [ %14, %land.1.L43C17.2 ], [ %14, %for.tail.L42.1 ]
  %43 = load i1, ptr %.fca.3.gep, align 1
  br i1 %43, label %for.tail.L42.3, label %land.1.L43C17.3

land.1.L43C17.3:                                  ; preds = %for.tail.L42.2
  %44 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 3
  %45 = load i32, ptr %44, align 4
  %.not.3 = icmp eq i32 %45, 0
  br i1 %.not.3, label %for.tail.L42.3, label %land.2.L43C17.3

land.2.L43C17.3:                                  ; preds = %land.1.L43C17.3
  %46 = load i32, ptr %24, align 4
  %.not33.3 = icmp eq i32 %46, %1
  br i1 %.not33.3, label %for.tail.L42.3, label %land.3.L43C17.3

land.3.L43C17.3:                                  ; preds = %land.2.L43C17.3
  %47 = add i32 %46, %45
  %spec.store.select.3 = tail call i32 @llvm.smin.i32(i32 %47, i32 %12)
  store i32 %spec.store.select.3, ptr %.fca.3.gep21, align 4
  br label %for.tail.L42.3

for.tail.L42.3:                                   ; preds = %land.3.L43C17.3, %land.2.L43C17.3, %land.1.L43C17.3, %for.tail.L42.2
  %48 = phi i32 [ %spec.store.select.3, %land.3.L43C17.3 ], [ %12, %land.2.L43C17.3 ], [ %12, %land.1.L43C17.3 ], [ %12, %for.tail.L42.2 ]
  %49 = load i1, ptr %.fca.4.gep, align 1
  br i1 %49, label %for.tail.L42.4, label %land.1.L43C17.4

land.1.L43C17.4:                                  ; preds = %for.tail.L42.3
  %50 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 4
  %51 = load i32, ptr %50, align 4
  %.not.4 = icmp eq i32 %51, 0
  br i1 %.not.4, label %for.tail.L42.4, label %land.2.L43C17.4

land.2.L43C17.4:                                  ; preds = %land.1.L43C17.4
  %52 = load i32, ptr %24, align 4
  %.not33.4 = icmp eq i32 %52, %1
  br i1 %.not33.4, label %for.tail.L42.4, label %land.3.L43C17.4

land.3.L43C17.4:                                  ; preds = %land.2.L43C17.4
  %53 = add i32 %52, %51
  %spec.store.select.4 = tail call i32 @llvm.smin.i32(i32 %53, i32 %10)
  store i32 %spec.store.select.4, ptr %.fca.4.gep22, align 4
  br label %for.tail.L42.4

for.tail.L42.4:                                   ; preds = %land.3.L43C17.4, %land.2.L43C17.4, %land.1.L43C17.4, %for.tail.L42.3
  %54 = phi i32 [ %spec.store.select.4, %land.3.L43C17.4 ], [ %10, %land.2.L43C17.4 ], [ %10, %land.1.L43C17.4 ], [ %10, %for.tail.L42.3 ]
  %55 = load i1, ptr %.fca.5.gep, align 1
  br i1 %55, label %for.tail.L42.5, label %land.1.L43C17.5

land.1.L43C17.5:                                  ; preds = %for.tail.L42.4
  %56 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 5
  %57 = load i32, ptr %56, align 4
  %.not.5 = icmp eq i32 %57, 0
  br i1 %.not.5, label %for.tail.L42.5, label %land.2.L43C17.5

land.2.L43C17.5:                                  ; preds = %land.1.L43C17.5
  %58 = load i32, ptr %24, align 4
  %.not33.5 = icmp eq i32 %58, %1
  br i1 %.not33.5, label %for.tail.L42.5, label %land.3.L43C17.5

land.3.L43C17.5:                                  ; preds = %land.2.L43C17.5
  %59 = add i32 %58, %57
  %spec.store.select.5 = tail call i32 @llvm.smin.i32(i32 %59, i32 %8)
  store i32 %spec.store.select.5, ptr %.fca.5.gep23, align 4
  br label %for.tail.L42.5

for.tail.L42.5:                                   ; preds = %land.3.L43C17.5, %land.2.L43C17.5, %land.1.L43C17.5, %for.tail.L42.4
  %60 = phi i32 [ %spec.store.select.5, %land.3.L43C17.5 ], [ %8, %land.2.L43C17.5 ], [ %8, %land.1.L43C17.5 ], [ %8, %for.tail.L42.4 ]
  %61 = load i1, ptr %.fca.6.gep, align 1
  br i1 %61, label %for.tail.L42.6, label %land.1.L43C17.6

land.1.L43C17.6:                                  ; preds = %for.tail.L42.5
  %62 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 6
  %63 = load i32, ptr %62, align 4
  %.not.6 = icmp eq i32 %63, 0
  br i1 %.not.6, label %for.tail.L42.6, label %land.2.L43C17.6

land.2.L43C17.6:                                  ; preds = %land.1.L43C17.6
  %64 = load i32, ptr %24, align 4
  %.not33.6 = icmp eq i32 %64, %1
  br i1 %.not33.6, label %for.tail.L42.6, label %land.3.L43C17.6

land.3.L43C17.6:                                  ; preds = %land.2.L43C17.6
  %65 = add i32 %64, %63
  %spec.store.select.6 = tail call i32 @llvm.smin.i32(i32 %65, i32 %6)
  store i32 %spec.store.select.6, ptr %.fca.6.gep24, align 4
  br label %for.tail.L42.6

for.tail.L42.6:                                   ; preds = %land.3.L43C17.6, %land.2.L43C17.6, %land.1.L43C17.6, %for.tail.L42.5
  %66 = phi i32 [ %spec.store.select.6, %land.3.L43C17.6 ], [ %6, %land.2.L43C17.6 ], [ %6, %land.1.L43C17.6 ], [ %6, %for.tail.L42.5 ]
  %67 = load i1, ptr %.fca.7.gep, align 1
  br i1 %67, label %for.tail.L42.7, label %land.1.L43C17.7

land.1.L43C17.7:                                  ; preds = %for.tail.L42.6
  %68 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 7
  %69 = load i32, ptr %68, align 4
  %.not.7 = icmp eq i32 %69, 0
  br i1 %.not.7, label %for.tail.L42.7, label %land.2.L43C17.7

land.2.L43C17.7:                                  ; preds = %land.1.L43C17.7
  %70 = load i32, ptr %24, align 4
  %.not33.7 = icmp eq i32 %70, %1
  br i1 %.not33.7, label %for.tail.L42.7, label %land.3.L43C17.7

land.3.L43C17.7:                                  ; preds = %land.2.L43C17.7
  %71 = add i32 %70, %69
  %spec.store.select.7 = tail call i32 @llvm.smin.i32(i32 %71, i32 %4)
  store i32 %spec.store.select.7, ptr %.fca.7.gep25, align 4
  br label %for.tail.L42.7

for.tail.L42.7:                                   ; preds = %land.3.L43C17.7, %land.2.L43C17.7, %land.1.L43C17.7, %for.tail.L42.6
  %72 = phi i32 [ %spec.store.select.7, %land.3.L43C17.7 ], [ %4, %land.2.L43C17.7 ], [ %4, %land.1.L43C17.7 ], [ %4, %for.tail.L42.6 ]
  %73 = load i1, ptr %.fca.8.gep, align 1
  br i1 %73, label %for.tail.L42.8, label %land.1.L43C17.8

land.1.L43C17.8:                                  ; preds = %for.tail.L42.7
  %74 = getelementptr inbounds [9 x i32], ptr %0, i64 %22, i64 8
  %75 = load i32, ptr %74, align 4
  %.not.8 = icmp eq i32 %75, 0
  br i1 %.not.8, label %for.tail.L42.8, label %land.2.L43C17.8

land.2.L43C17.8:                                  ; preds = %land.1.L43C17.8
  %76 = load i32, ptr %24, align 4
  %.not33.8 = icmp eq i32 %76, %1
  br i1 %.not33.8, label %for.tail.L42.8, label %land.3.L43C17.8

land.3.L43C17.8:                                  ; preds = %land.2.L43C17.8
  %77 = add i32 %76, %75
  %spec.store.select.8 = tail call i32 @llvm.smin.i32(i32 %77, i32 %2)
  store i32 %spec.store.select.8, ptr %.fca.8.gep26, align 4
  br label %for.tail.L42.8

for.tail.L42.8:                                   ; preds = %land.3.L43C17.8, %land.2.L43C17.8, %land.1.L43C17.8, %for.tail.L42.7
  %78 = phi i32 [ %spec.store.select.8, %land.3.L43C17.8 ], [ %2, %land.2.L43C17.8 ], [ %2, %land.1.L43C17.8 ], [ %2, %for.tail.L42.7 ]
  %spec.select7.i.1 = tail call i32 @llvm.smin.i32(i32 %30, i32 %1)
  %min.1.i.1 = select i1 %25, i32 %1, i32 %spec.select7.i.1
  br i1 %31, label %for.tail.L9.1.i.1, label %land.1.L10C12.1.i.1

land.1.L10C12.1.i.1:                              ; preds = %for.tail.L42.8
  %.not.1.i.1 = icmp sle i32 %36, %min.1.i.1
  %spec.select.1.i.1 = zext i1 %.not.1.i.1 to i32
  %spec.select7.1.i.1 = tail call i32 @llvm.smin.i32(i32 %36, i32 %min.1.i.1)
  br label %for.tail.L9.1.i.1

for.tail.L9.1.i.1:                                ; preds = %land.1.L10C12.1.i.1, %for.tail.L42.8
  %minIndex.1.1.i.1 = phi i32 [ 0, %for.tail.L42.8 ], [ %spec.select.1.i.1, %land.1.L10C12.1.i.1 ]
  %min.1.1.i.1 = phi i32 [ %min.1.i.1, %for.tail.L42.8 ], [ %spec.select7.1.i.1, %land.1.L10C12.1.i.1 ]
  br i1 %37, label %for.tail.L9.2.i.1, label %land.1.L10C12.2.i.1

land.1.L10C12.2.i.1:                              ; preds = %for.tail.L9.1.i.1
  %.not.2.i.1 = icmp sgt i32 %42, %min.1.1.i.1
  %spec.select.2.i.1 = select i1 %.not.2.i.1, i32 %minIndex.1.1.i.1, i32 2
  %spec.select7.2.i.1 = tail call i32 @llvm.smin.i32(i32 %42, i32 %min.1.1.i.1)
  br label %for.tail.L9.2.i.1

for.tail.L9.2.i.1:                                ; preds = %land.1.L10C12.2.i.1, %for.tail.L9.1.i.1
  %minIndex.1.2.i.1 = phi i32 [ %minIndex.1.1.i.1, %for.tail.L9.1.i.1 ], [ %spec.select.2.i.1, %land.1.L10C12.2.i.1 ]
  %min.1.2.i.1 = phi i32 [ %min.1.1.i.1, %for.tail.L9.1.i.1 ], [ %spec.select7.2.i.1, %land.1.L10C12.2.i.1 ]
  br i1 %43, label %for.tail.L9.3.i.1, label %land.1.L10C12.3.i.1

land.1.L10C12.3.i.1:                              ; preds = %for.tail.L9.2.i.1
  %.not.3.i.1 = icmp sgt i32 %48, %min.1.2.i.1
  %spec.select.3.i.1 = select i1 %.not.3.i.1, i32 %minIndex.1.2.i.1, i32 3
  %spec.select7.3.i.1 = tail call i32 @llvm.smin.i32(i32 %48, i32 %min.1.2.i.1)
  br label %for.tail.L9.3.i.1

for.tail.L9.3.i.1:                                ; preds = %land.1.L10C12.3.i.1, %for.tail.L9.2.i.1
  %minIndex.1.3.i.1 = phi i32 [ %minIndex.1.2.i.1, %for.tail.L9.2.i.1 ], [ %spec.select.3.i.1, %land.1.L10C12.3.i.1 ]
  %min.1.3.i.1 = phi i32 [ %min.1.2.i.1, %for.tail.L9.2.i.1 ], [ %spec.select7.3.i.1, %land.1.L10C12.3.i.1 ]
  br i1 %49, label %for.tail.L9.4.i.1, label %land.1.L10C12.4.i.1

land.1.L10C12.4.i.1:                              ; preds = %for.tail.L9.3.i.1
  %.not.4.i.1 = icmp sgt i32 %54, %min.1.3.i.1
  %spec.select.4.i.1 = select i1 %.not.4.i.1, i32 %minIndex.1.3.i.1, i32 4
  %spec.select7.4.i.1 = tail call i32 @llvm.smin.i32(i32 %54, i32 %min.1.3.i.1)
  br label %for.tail.L9.4.i.1

for.tail.L9.4.i.1:                                ; preds = %land.1.L10C12.4.i.1, %for.tail.L9.3.i.1
  %minIndex.1.4.i.1 = phi i32 [ %minIndex.1.3.i.1, %for.tail.L9.3.i.1 ], [ %spec.select.4.i.1, %land.1.L10C12.4.i.1 ]
  %min.1.4.i.1 = phi i32 [ %min.1.3.i.1, %for.tail.L9.3.i.1 ], [ %spec.select7.4.i.1, %land.1.L10C12.4.i.1 ]
  br i1 %55, label %for.tail.L9.5.i.1, label %land.1.L10C12.5.i.1

land.1.L10C12.5.i.1:                              ; preds = %for.tail.L9.4.i.1
  %.not.5.i.1 = icmp sgt i32 %60, %min.1.4.i.1
  %spec.select.5.i.1 = select i1 %.not.5.i.1, i32 %minIndex.1.4.i.1, i32 5
  %spec.select7.5.i.1 = tail call i32 @llvm.smin.i32(i32 %60, i32 %min.1.4.i.1)
  br label %for.tail.L9.5.i.1

for.tail.L9.5.i.1:                                ; preds = %land.1.L10C12.5.i.1, %for.tail.L9.4.i.1
  %minIndex.1.5.i.1 = phi i32 [ %minIndex.1.4.i.1, %for.tail.L9.4.i.1 ], [ %spec.select.5.i.1, %land.1.L10C12.5.i.1 ]
  %min.1.5.i.1 = phi i32 [ %min.1.4.i.1, %for.tail.L9.4.i.1 ], [ %spec.select7.5.i.1, %land.1.L10C12.5.i.1 ]
  br i1 %61, label %for.tail.L9.6.i.1, label %land.1.L10C12.6.i.1

land.1.L10C12.6.i.1:                              ; preds = %for.tail.L9.5.i.1
  %.not.6.i.1 = icmp sgt i32 %66, %min.1.5.i.1
  %spec.select.6.i.1 = select i1 %.not.6.i.1, i32 %minIndex.1.5.i.1, i32 6
  %spec.select7.6.i.1 = tail call i32 @llvm.smin.i32(i32 %66, i32 %min.1.5.i.1)
  br label %for.tail.L9.6.i.1

for.tail.L9.6.i.1:                                ; preds = %land.1.L10C12.6.i.1, %for.tail.L9.5.i.1
  %minIndex.1.6.i.1 = phi i32 [ %minIndex.1.5.i.1, %for.tail.L9.5.i.1 ], [ %spec.select.6.i.1, %land.1.L10C12.6.i.1 ]
  %min.1.6.i.1 = phi i32 [ %min.1.5.i.1, %for.tail.L9.5.i.1 ], [ %spec.select7.6.i.1, %land.1.L10C12.6.i.1 ]
  br i1 %67, label %for.tail.L9.7.i.1, label %land.1.L10C12.7.i.1

land.1.L10C12.7.i.1:                              ; preds = %for.tail.L9.6.i.1
  %.not.7.i.1 = icmp sgt i32 %72, %min.1.6.i.1
  %spec.select.7.i.1 = select i1 %.not.7.i.1, i32 %minIndex.1.6.i.1, i32 7
  %spec.select7.7.i.1 = tail call i32 @llvm.smin.i32(i32 %72, i32 %min.1.6.i.1)
  br label %for.tail.L9.7.i.1

for.tail.L9.7.i.1:                                ; preds = %land.1.L10C12.7.i.1, %for.tail.L9.6.i.1
  %minIndex.1.7.i.1 = phi i32 [ %minIndex.1.6.i.1, %for.tail.L9.6.i.1 ], [ %spec.select.7.i.1, %land.1.L10C12.7.i.1 ]
  %min.1.7.i.1 = phi i32 [ %min.1.6.i.1, %for.tail.L9.6.i.1 ], [ %spec.select7.7.i.1, %land.1.L10C12.7.i.1 ]
  %.not.8.i.1 = icmp sgt i32 %78, %min.1.7.i.1
  %79 = select i1 %73, i1 true, i1 %.not.8.i.1
  %80 = zext i32 %minIndex.1.7.i.1 to i64
  %81 = select i1 %79, i64 %80, i64 8
  %82 = getelementptr inbounds [9 x i1], ptr %sptSet, i64 0, i64 %81
  store i1 true, ptr %82, align 1
  %83 = getelementptr inbounds [9 x i32], ptr %dist, i64 0, i64 %81
  %84 = load i1, ptr %sptSet, align 1
  br i1 %84, label %for.tail.L42.123, label %land.1.L43C17.118

land.1.L43C17.118:                                ; preds = %for.tail.L9.7.i.1
  %85 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 0
  %86 = load i32, ptr %85, align 4
  %.not.117 = icmp eq i32 %86, 0
  br i1 %.not.117, label %for.tail.L42.123, label %land.2.L43C17.120

land.2.L43C17.120:                                ; preds = %land.1.L43C17.118
  %87 = load i32, ptr %83, align 4
  %.not33.119 = icmp eq i32 %87, %1
  br i1 %.not33.119, label %for.tail.L42.123, label %land.3.L43C17.122

land.3.L43C17.122:                                ; preds = %land.2.L43C17.120
  %88 = add i32 %87, %86
  %spec.store.select.121 = tail call i32 @llvm.smin.i32(i32 %88, i32 %30)
  store i32 %spec.store.select.121, ptr %dist, align 4
  br label %for.tail.L42.123

for.tail.L42.123:                                 ; preds = %land.3.L43C17.122, %land.2.L43C17.120, %land.1.L43C17.118, %for.tail.L9.7.i.1
  %89 = phi i32 [ %spec.store.select.121, %land.3.L43C17.122 ], [ %30, %for.tail.L9.7.i.1 ], [ %30, %land.1.L43C17.118 ], [ %30, %land.2.L43C17.120 ]
  %90 = load i1, ptr %.fca.1.gep, align 1
  br i1 %90, label %for.tail.L42.1.1, label %land.1.L43C17.1.1

land.1.L43C17.1.1:                                ; preds = %for.tail.L42.123
  %91 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 1
  %92 = load i32, ptr %91, align 4
  %.not.1.1 = icmp eq i32 %92, 0
  br i1 %.not.1.1, label %for.tail.L42.1.1, label %land.2.L43C17.1.1

land.2.L43C17.1.1:                                ; preds = %land.1.L43C17.1.1
  %93 = load i32, ptr %83, align 4
  %.not33.1.1 = icmp eq i32 %93, %1
  br i1 %.not33.1.1, label %for.tail.L42.1.1, label %land.3.L43C17.1.1

land.3.L43C17.1.1:                                ; preds = %land.2.L43C17.1.1
  %94 = add i32 %93, %92
  %spec.store.select.1.1 = tail call i32 @llvm.smin.i32(i32 %94, i32 %36)
  store i32 %spec.store.select.1.1, ptr %.fca.1.gep19, align 4
  br label %for.tail.L42.1.1

for.tail.L42.1.1:                                 ; preds = %land.3.L43C17.1.1, %land.2.L43C17.1.1, %land.1.L43C17.1.1, %for.tail.L42.123
  %95 = phi i32 [ %spec.store.select.1.1, %land.3.L43C17.1.1 ], [ %36, %land.2.L43C17.1.1 ], [ %36, %land.1.L43C17.1.1 ], [ %36, %for.tail.L42.123 ]
  %96 = load i1, ptr %.fca.2.gep, align 1
  br i1 %96, label %for.tail.L42.2.1, label %land.1.L43C17.2.1

land.1.L43C17.2.1:                                ; preds = %for.tail.L42.1.1
  %97 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 2
  %98 = load i32, ptr %97, align 4
  %.not.2.1 = icmp eq i32 %98, 0
  br i1 %.not.2.1, label %for.tail.L42.2.1, label %land.2.L43C17.2.1

land.2.L43C17.2.1:                                ; preds = %land.1.L43C17.2.1
  %99 = load i32, ptr %83, align 4
  %.not33.2.1 = icmp eq i32 %99, %1
  br i1 %.not33.2.1, label %for.tail.L42.2.1, label %land.3.L43C17.2.1

land.3.L43C17.2.1:                                ; preds = %land.2.L43C17.2.1
  %100 = add i32 %99, %98
  %spec.store.select.2.1 = tail call i32 @llvm.smin.i32(i32 %100, i32 %42)
  store i32 %spec.store.select.2.1, ptr %.fca.2.gep20, align 4
  br label %for.tail.L42.2.1

for.tail.L42.2.1:                                 ; preds = %land.3.L43C17.2.1, %land.2.L43C17.2.1, %land.1.L43C17.2.1, %for.tail.L42.1.1
  %101 = phi i32 [ %spec.store.select.2.1, %land.3.L43C17.2.1 ], [ %42, %land.2.L43C17.2.1 ], [ %42, %land.1.L43C17.2.1 ], [ %42, %for.tail.L42.1.1 ]
  %102 = load i1, ptr %.fca.3.gep, align 1
  br i1 %102, label %for.tail.L42.3.1, label %land.1.L43C17.3.1

land.1.L43C17.3.1:                                ; preds = %for.tail.L42.2.1
  %103 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 3
  %104 = load i32, ptr %103, align 4
  %.not.3.1 = icmp eq i32 %104, 0
  br i1 %.not.3.1, label %for.tail.L42.3.1, label %land.2.L43C17.3.1

land.2.L43C17.3.1:                                ; preds = %land.1.L43C17.3.1
  %105 = load i32, ptr %83, align 4
  %.not33.3.1 = icmp eq i32 %105, %1
  br i1 %.not33.3.1, label %for.tail.L42.3.1, label %land.3.L43C17.3.1

land.3.L43C17.3.1:                                ; preds = %land.2.L43C17.3.1
  %106 = add i32 %105, %104
  %spec.store.select.3.1 = tail call i32 @llvm.smin.i32(i32 %106, i32 %48)
  store i32 %spec.store.select.3.1, ptr %.fca.3.gep21, align 4
  br label %for.tail.L42.3.1

for.tail.L42.3.1:                                 ; preds = %land.3.L43C17.3.1, %land.2.L43C17.3.1, %land.1.L43C17.3.1, %for.tail.L42.2.1
  %107 = phi i32 [ %spec.store.select.3.1, %land.3.L43C17.3.1 ], [ %48, %land.2.L43C17.3.1 ], [ %48, %land.1.L43C17.3.1 ], [ %48, %for.tail.L42.2.1 ]
  %108 = load i1, ptr %.fca.4.gep, align 1
  br i1 %108, label %for.tail.L42.4.1, label %land.1.L43C17.4.1

land.1.L43C17.4.1:                                ; preds = %for.tail.L42.3.1
  %109 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 4
  %110 = load i32, ptr %109, align 4
  %.not.4.1 = icmp eq i32 %110, 0
  br i1 %.not.4.1, label %for.tail.L42.4.1, label %land.2.L43C17.4.1

land.2.L43C17.4.1:                                ; preds = %land.1.L43C17.4.1
  %111 = load i32, ptr %83, align 4
  %.not33.4.1 = icmp eq i32 %111, %1
  br i1 %.not33.4.1, label %for.tail.L42.4.1, label %land.3.L43C17.4.1

land.3.L43C17.4.1:                                ; preds = %land.2.L43C17.4.1
  %112 = add i32 %111, %110
  %spec.store.select.4.1 = tail call i32 @llvm.smin.i32(i32 %112, i32 %54)
  store i32 %spec.store.select.4.1, ptr %.fca.4.gep22, align 4
  br label %for.tail.L42.4.1

for.tail.L42.4.1:                                 ; preds = %land.3.L43C17.4.1, %land.2.L43C17.4.1, %land.1.L43C17.4.1, %for.tail.L42.3.1
  %113 = phi i32 [ %spec.store.select.4.1, %land.3.L43C17.4.1 ], [ %54, %land.2.L43C17.4.1 ], [ %54, %land.1.L43C17.4.1 ], [ %54, %for.tail.L42.3.1 ]
  %114 = load i1, ptr %.fca.5.gep, align 1
  br i1 %114, label %for.tail.L42.5.1, label %land.1.L43C17.5.1

land.1.L43C17.5.1:                                ; preds = %for.tail.L42.4.1
  %115 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 5
  %116 = load i32, ptr %115, align 4
  %.not.5.1 = icmp eq i32 %116, 0
  br i1 %.not.5.1, label %for.tail.L42.5.1, label %land.2.L43C17.5.1

land.2.L43C17.5.1:                                ; preds = %land.1.L43C17.5.1
  %117 = load i32, ptr %83, align 4
  %.not33.5.1 = icmp eq i32 %117, %1
  br i1 %.not33.5.1, label %for.tail.L42.5.1, label %land.3.L43C17.5.1

land.3.L43C17.5.1:                                ; preds = %land.2.L43C17.5.1
  %118 = add i32 %117, %116
  %spec.store.select.5.1 = tail call i32 @llvm.smin.i32(i32 %118, i32 %60)
  store i32 %spec.store.select.5.1, ptr %.fca.5.gep23, align 4
  br label %for.tail.L42.5.1

for.tail.L42.5.1:                                 ; preds = %land.3.L43C17.5.1, %land.2.L43C17.5.1, %land.1.L43C17.5.1, %for.tail.L42.4.1
  %119 = phi i32 [ %spec.store.select.5.1, %land.3.L43C17.5.1 ], [ %60, %land.2.L43C17.5.1 ], [ %60, %land.1.L43C17.5.1 ], [ %60, %for.tail.L42.4.1 ]
  %120 = load i1, ptr %.fca.6.gep, align 1
  br i1 %120, label %for.tail.L42.6.1, label %land.1.L43C17.6.1

land.1.L43C17.6.1:                                ; preds = %for.tail.L42.5.1
  %121 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 6
  %122 = load i32, ptr %121, align 4
  %.not.6.1 = icmp eq i32 %122, 0
  br i1 %.not.6.1, label %for.tail.L42.6.1, label %land.2.L43C17.6.1

land.2.L43C17.6.1:                                ; preds = %land.1.L43C17.6.1
  %123 = load i32, ptr %83, align 4
  %.not33.6.1 = icmp eq i32 %123, %1
  br i1 %.not33.6.1, label %for.tail.L42.6.1, label %land.3.L43C17.6.1

land.3.L43C17.6.1:                                ; preds = %land.2.L43C17.6.1
  %124 = add i32 %123, %122
  %spec.store.select.6.1 = tail call i32 @llvm.smin.i32(i32 %124, i32 %66)
  store i32 %spec.store.select.6.1, ptr %.fca.6.gep24, align 4
  br label %for.tail.L42.6.1

for.tail.L42.6.1:                                 ; preds = %land.3.L43C17.6.1, %land.2.L43C17.6.1, %land.1.L43C17.6.1, %for.tail.L42.5.1
  %125 = phi i32 [ %spec.store.select.6.1, %land.3.L43C17.6.1 ], [ %66, %land.2.L43C17.6.1 ], [ %66, %land.1.L43C17.6.1 ], [ %66, %for.tail.L42.5.1 ]
  %126 = load i1, ptr %.fca.7.gep, align 1
  br i1 %126, label %for.tail.L42.7.1, label %land.1.L43C17.7.1

land.1.L43C17.7.1:                                ; preds = %for.tail.L42.6.1
  %127 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 7
  %128 = load i32, ptr %127, align 4
  %.not.7.1 = icmp eq i32 %128, 0
  br i1 %.not.7.1, label %for.tail.L42.7.1, label %land.2.L43C17.7.1

land.2.L43C17.7.1:                                ; preds = %land.1.L43C17.7.1
  %129 = load i32, ptr %83, align 4
  %.not33.7.1 = icmp eq i32 %129, %1
  br i1 %.not33.7.1, label %for.tail.L42.7.1, label %land.3.L43C17.7.1

land.3.L43C17.7.1:                                ; preds = %land.2.L43C17.7.1
  %130 = add i32 %129, %128
  %spec.store.select.7.1 = tail call i32 @llvm.smin.i32(i32 %130, i32 %72)
  store i32 %spec.store.select.7.1, ptr %.fca.7.gep25, align 4
  br label %for.tail.L42.7.1

for.tail.L42.7.1:                                 ; preds = %land.3.L43C17.7.1, %land.2.L43C17.7.1, %land.1.L43C17.7.1, %for.tail.L42.6.1
  %131 = phi i32 [ %spec.store.select.7.1, %land.3.L43C17.7.1 ], [ %72, %land.2.L43C17.7.1 ], [ %72, %land.1.L43C17.7.1 ], [ %72, %for.tail.L42.6.1 ]
  %132 = load i1, ptr %.fca.8.gep, align 1
  br i1 %132, label %for.tail.L42.8.1, label %land.1.L43C17.8.1

land.1.L43C17.8.1:                                ; preds = %for.tail.L42.7.1
  %133 = getelementptr inbounds [9 x i32], ptr %0, i64 %81, i64 8
  %134 = load i32, ptr %133, align 4
  %.not.8.1 = icmp eq i32 %134, 0
  br i1 %.not.8.1, label %for.tail.L42.8.1, label %land.2.L43C17.8.1

land.2.L43C17.8.1:                                ; preds = %land.1.L43C17.8.1
  %135 = load i32, ptr %83, align 4
  %.not33.8.1 = icmp eq i32 %135, %1
  br i1 %.not33.8.1, label %for.tail.L42.8.1, label %land.3.L43C17.8.1

land.3.L43C17.8.1:                                ; preds = %land.2.L43C17.8.1
  %136 = add i32 %135, %134
  %spec.store.select.8.1 = tail call i32 @llvm.smin.i32(i32 %136, i32 %78)
  store i32 %spec.store.select.8.1, ptr %.fca.8.gep26, align 4
  br label %for.tail.L42.8.1

for.tail.L42.8.1:                                 ; preds = %land.3.L43C17.8.1, %land.2.L43C17.8.1, %land.1.L43C17.8.1, %for.tail.L42.7.1
  %137 = phi i32 [ %spec.store.select.8.1, %land.3.L43C17.8.1 ], [ %78, %land.2.L43C17.8.1 ], [ %78, %land.1.L43C17.8.1 ], [ %78, %for.tail.L42.7.1 ]
  %138 = add nuw nsw i32 %count.036, 2
  %exitcond.not.1 = icmp eq i32 %138, 8
  br i1 %exitcond.not.1, label %for.exit.L39, label %for.body.L39

for.exit.L39:                                     ; preds = %for.tail.L42.8.1
  %puts.i = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %139 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 0, i32 %89)
  %140 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 1, i32 %95)
  %141 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 2, i32 %101)
  %142 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 3, i32 %107)
  %143 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 4, i32 %113)
  %144 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 5, i32 %119)
  %145 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 6, i32 %125)
  %146 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 7, i32 %131)
  %147 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 8, i32 %137)
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
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  call fastcc void @_p__void__void__dijkstra__intarrayarray_int(ptr nonnull %graph) #4
  %puts1 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #4 = { nounwind }
