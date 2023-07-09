; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@printf.str.2 = private unnamed_addr constant [23 x i8] c"No solution was found.\00", align 1

; Function Attrs: nofree nounwind
define private fastcc void @_Z5printPA9A9i(ptr nocapture readonly %0) unnamed_addr #0 {
  br label %for.head.L5.preheader

for.head.L5.preheader:                            ; preds = %1, %for.head.L5.preheader
  %indvars.iv = phi i64 [ 0, %1 ], [ %indvars.iv.next, %for.head.L5.preheader ]
  %2 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 0
  %3 = load i32, ptr %2, align 4
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %3)
  %5 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 1
  %6 = load i32, ptr %5, align 4
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %6)
  %8 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 2
  %9 = load i32, ptr %8, align 4
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %9)
  %11 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 3
  %12 = load i32, ptr %11, align 4
  %13 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %12)
  %14 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 4
  %15 = load i32, ptr %14, align 4
  %16 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %15)
  %17 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 5
  %18 = load i32, ptr %17, align 4
  %19 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %18)
  %20 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 6
  %21 = load i32, ptr %20, align 4
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %21)
  %23 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 7
  %24 = load i32, ptr %23, align 4
  %25 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %24)
  %26 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %indvars.iv, i64 8
  %27 = load i32, ptr %26, align 4
  %28 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %27)
  %putchar = tail call i32 @putchar(i32 10)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 9
  br i1 %exitcond.not, label %for.exit.L4, label %for.head.L5.preheader

for.exit.L4:                                      ; preds = %for.head.L5.preheader
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind memory(argmem: readwrite)
define private fastcc i1 @_Z11solveSudokuPA9A9iii(ptr %0, i32 %1, i32 %2) unnamed_addr #1 {
land.exit.L45C8:
  %3 = icmp eq i32 %1, 8
  %4 = icmp eq i32 %2, 9
  %land_phi30 = select i1 %3, i1 %4, i1 false
  br i1 %land_phi30, label %common.ret, label %if.exit.L45

common.ret:                                       ; preds = %if.then.L52, %if.exit.L57, %if.then.L57, %land.exit.L45C8
  %common.ret.op = phi i1 [ true, %land.exit.L45C8 ], [ false, %if.exit.L57 ], [ true, %if.then.L57 ], [ true, %if.then.L52 ]
  ret i1 %common.ret.op

if.exit.L45:                                      ; preds = %land.exit.L45C8, %if.then.L52
  %5 = phi i1 [ %56, %if.then.L52 ], [ %4, %land.exit.L45C8 ]
  %.tr2632 = phi i32 [ %54, %if.then.L52 ], [ %2, %land.exit.L45C8 ]
  %.tr2531 = phi i32 [ %spec.select, %if.then.L52 ], [ %1, %land.exit.L45C8 ]
  %6 = zext i1 %5 to i32
  %spec.select = add i32 %.tr2531, %6
  %spec.select23 = select i1 %5, i32 0, i32 %.tr2632
  %7 = sext i32 %spec.select to i64
  %8 = sext i32 %spec.select23 to i64
  %9 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = icmp sgt i32 %10, 0
  br i1 %11, label %if.then.L52, label %for.head.L56.preheader

for.head.L56.preheader:                           ; preds = %if.exit.L45
  %12 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 0
  %13 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 1
  %14 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 2
  %15 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 3
  %16 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 4
  %17 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 5
  %18 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 6
  %19 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 7
  %20 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %7, i64 8
  %21 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 0, i64 %8
  %22 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 1, i64 %8
  %23 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 2, i64 %8
  %24 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 3, i64 %8
  %25 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 4, i64 %8
  %26 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 5, i64 %8
  %27 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 6, i64 %8
  %28 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 7, i64 %8
  %29 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 8, i64 %8
  %30 = srem i32 %spec.select, 3
  %31 = sub i32 %spec.select, %30
  %32 = srem i32 %spec.select23, 3
  %33 = sub i32 %spec.select23, %32
  %34 = sext i32 %31 to i64
  %35 = sext i32 %33 to i64
  %36 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %34, i64 %35
  %37 = add i32 %33, 1
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %34, i64 %38
  %40 = add i32 %33, 2
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %34, i64 %41
  %43 = add i32 %31, 1
  %44 = sext i32 %43 to i64
  %45 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %44, i64 %35
  %46 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %44, i64 %38
  %47 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %44, i64 %41
  %48 = add i32 %31, 2
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %49, i64 %35
  %51 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %49, i64 %38
  %52 = getelementptr inbounds [9 x [9 x i32]], ptr %0, i64 0, i64 %49, i64 %41
  %53 = add i32 %spec.select23, 1
  br label %for.body.L56

if.then.L52:                                      ; preds = %if.exit.L45
  %54 = add i32 %spec.select23, 1
  %55 = icmp eq i32 %spec.select, 8
  %56 = icmp eq i32 %54, 9
  %land_phi = select i1 %55, i1 %56, i1 false
  br i1 %land_phi, label %common.ret, label %if.exit.L45

for.body.L56:                                     ; preds = %for.head.L56.preheader, %if.exit.L57
  %num.033 = phi i32 [ 1, %for.head.L56.preheader ], [ %111, %if.exit.L57 ]
  %57 = load i32, ptr %12, align 4
  %58 = icmp eq i32 %57, %num.033
  br i1 %58, label %if.exit.L57, label %for.head.L14.i

for.head.L14.i:                                   ; preds = %for.body.L56
  %59 = load i32, ptr %13, align 4
  %60 = icmp eq i32 %59, %num.033
  br i1 %60, label %if.exit.L57, label %for.head.L14.1.i

for.head.L14.1.i:                                 ; preds = %for.head.L14.i
  %61 = load i32, ptr %14, align 4
  %62 = icmp eq i32 %61, %num.033
  br i1 %62, label %if.exit.L57, label %for.head.L14.2.i

for.head.L14.2.i:                                 ; preds = %for.head.L14.1.i
  %63 = load i32, ptr %15, align 4
  %64 = icmp eq i32 %63, %num.033
  br i1 %64, label %if.exit.L57, label %for.head.L14.3.i

for.head.L14.3.i:                                 ; preds = %for.head.L14.2.i
  %65 = load i32, ptr %16, align 4
  %66 = icmp eq i32 %65, %num.033
  br i1 %66, label %if.exit.L57, label %for.head.L14.4.i

for.head.L14.4.i:                                 ; preds = %for.head.L14.3.i
  %67 = load i32, ptr %17, align 4
  %68 = icmp eq i32 %67, %num.033
  br i1 %68, label %if.exit.L57, label %for.head.L14.5.i

for.head.L14.5.i:                                 ; preds = %for.head.L14.4.i
  %69 = load i32, ptr %18, align 4
  %70 = icmp eq i32 %69, %num.033
  br i1 %70, label %if.exit.L57, label %for.head.L14.6.i

for.head.L14.6.i:                                 ; preds = %for.head.L14.5.i
  %71 = load i32, ptr %19, align 4
  %72 = icmp eq i32 %71, %num.033
  br i1 %72, label %if.exit.L57, label %for.head.L14.7.i

for.head.L14.7.i:                                 ; preds = %for.head.L14.6.i
  %73 = load i32, ptr %20, align 4
  %74 = icmp eq i32 %73, %num.033
  br i1 %74, label %if.exit.L57, label %for.head.L14.8.i

for.head.L14.8.i:                                 ; preds = %for.head.L14.7.i
  %75 = load i32, ptr %21, align 4
  %76 = icmp eq i32 %75, %num.033
  br i1 %76, label %if.exit.L57, label %for.head.L21.i

for.head.L21.i:                                   ; preds = %for.head.L14.8.i
  %77 = load i32, ptr %22, align 4
  %78 = icmp eq i32 %77, %num.033
  br i1 %78, label %if.exit.L57, label %for.head.L21.1.i

for.head.L21.1.i:                                 ; preds = %for.head.L21.i
  %79 = load i32, ptr %23, align 4
  %80 = icmp eq i32 %79, %num.033
  br i1 %80, label %if.exit.L57, label %for.head.L21.2.i

for.head.L21.2.i:                                 ; preds = %for.head.L21.1.i
  %81 = load i32, ptr %24, align 4
  %82 = icmp eq i32 %81, %num.033
  br i1 %82, label %if.exit.L57, label %for.head.L21.3.i

for.head.L21.3.i:                                 ; preds = %for.head.L21.2.i
  %83 = load i32, ptr %25, align 4
  %84 = icmp eq i32 %83, %num.033
  br i1 %84, label %if.exit.L57, label %for.head.L21.4.i

for.head.L21.4.i:                                 ; preds = %for.head.L21.3.i
  %85 = load i32, ptr %26, align 4
  %86 = icmp eq i32 %85, %num.033
  br i1 %86, label %if.exit.L57, label %for.head.L21.5.i

for.head.L21.5.i:                                 ; preds = %for.head.L21.4.i
  %87 = load i32, ptr %27, align 4
  %88 = icmp eq i32 %87, %num.033
  br i1 %88, label %if.exit.L57, label %for.head.L21.6.i

for.head.L21.6.i:                                 ; preds = %for.head.L21.5.i
  %89 = load i32, ptr %28, align 4
  %90 = icmp eq i32 %89, %num.033
  br i1 %90, label %if.exit.L57, label %for.head.L21.7.i

for.head.L21.7.i:                                 ; preds = %for.head.L21.6.i
  %91 = load i32, ptr %29, align 4
  %92 = icmp eq i32 %91, %num.033
  br i1 %92, label %if.exit.L57, label %for.head.L21.8.i

for.head.L21.8.i:                                 ; preds = %for.head.L21.7.i
  %93 = load i32, ptr %36, align 4
  %94 = icmp eq i32 %93, %num.033
  br i1 %94, label %if.exit.L57, label %for.head.L32.i

for.head.L32.i:                                   ; preds = %for.head.L21.8.i
  %95 = load i32, ptr %39, align 4
  %96 = icmp eq i32 %95, %num.033
  br i1 %96, label %if.exit.L57, label %for.head.L32.1.i

for.head.L32.1.i:                                 ; preds = %for.head.L32.i
  %97 = load i32, ptr %42, align 4
  %98 = icmp eq i32 %97, %num.033
  br i1 %98, label %if.exit.L57, label %for.head.L32.2.i

for.head.L32.2.i:                                 ; preds = %for.head.L32.1.i
  %99 = load i32, ptr %45, align 4
  %100 = icmp eq i32 %99, %num.033
  br i1 %100, label %if.exit.L57, label %for.head.L32.133.i

for.head.L32.133.i:                               ; preds = %for.head.L32.2.i
  %101 = load i32, ptr %46, align 4
  %102 = icmp eq i32 %101, %num.033
  br i1 %102, label %if.exit.L57, label %for.head.L32.1.1.i

for.head.L32.1.1.i:                               ; preds = %for.head.L32.133.i
  %103 = load i32, ptr %47, align 4
  %104 = icmp eq i32 %103, %num.033
  br i1 %104, label %if.exit.L57, label %for.head.L32.2.1.i

for.head.L32.2.1.i:                               ; preds = %for.head.L32.1.1.i
  %105 = load i32, ptr %50, align 4
  %106 = icmp eq i32 %105, %num.033
  br i1 %106, label %if.exit.L57, label %for.head.L32.234.i

for.head.L32.234.i:                               ; preds = %for.head.L32.2.1.i
  %107 = load i32, ptr %51, align 4
  %108 = icmp eq i32 %107, %num.033
  br i1 %108, label %if.exit.L57, label %_Z6isSafePA9A9iiii.exit

_Z6isSafePA9A9iiii.exit:                          ; preds = %for.head.L32.234.i
  %109 = load i32, ptr %52, align 4
  %.not = icmp eq i32 %109, %num.033
  br i1 %.not, label %if.exit.L57, label %if.then.L57

if.then.L57:                                      ; preds = %_Z6isSafePA9A9iiii.exit
  store i32 %num.033, ptr %9, align 4
  %110 = tail call fastcc i1 @_Z11solveSudokuPA9A9iii(ptr nonnull %0, i32 %spec.select, i32 %53)
  br i1 %110, label %common.ret, label %if.exit.L57

if.exit.L57:                                      ; preds = %for.body.L56, %for.head.L14.i, %for.head.L14.1.i, %for.head.L14.2.i, %for.head.L14.3.i, %for.head.L14.4.i, %for.head.L14.5.i, %for.head.L14.6.i, %for.head.L14.7.i, %for.head.L14.8.i, %for.head.L21.i, %for.head.L21.1.i, %for.head.L21.2.i, %for.head.L21.3.i, %for.head.L21.4.i, %for.head.L21.5.i, %for.head.L21.6.i, %for.head.L21.7.i, %for.head.L21.8.i, %for.head.L32.i, %for.head.L32.1.i, %for.head.L32.2.i, %for.head.L32.133.i, %for.head.L32.1.1.i, %for.head.L32.2.1.i, %for.head.L32.234.i, %if.then.L57, %_Z6isSafePA9A9iiii.exit
  store i32 0, ptr %9, align 4
  %111 = add nuw nsw i32 %num.033, 1
  %exitcond.not = icmp eq i32 %111, 10
  br i1 %exitcond.not, label %common.ret, label %for.body.L56
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
  %grid = alloca [9 x [9 x i32]], align 4
  store i32 3, ptr %grid, align 4
  %.fca.0.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 1
  store i32 0, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 2
  store i32 6, ptr %.fca.0.2.gep, align 4
  %.fca.0.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 3
  store i32 5, ptr %.fca.0.3.gep, align 4
  %.fca.0.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 4
  store i32 0, ptr %.fca.0.4.gep, align 4
  %.fca.0.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 5
  store i32 8, ptr %.fca.0.5.gep, align 4
  %.fca.0.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 6
  store i32 4, ptr %.fca.0.6.gep, align 4
  %.fca.0.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 7
  store i32 0, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 8
  store i32 0, ptr %.fca.0.8.gep, align 4
  %.fca.1.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 0
  store i32 5, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 1
  store i32 2, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 2
  %.fca.2.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 1
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(32) %.fca.1.2.gep, i8 0, i64 32, i1 false)
  store i32 8, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 2
  store i32 7, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 3
  %.fca.2.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 7
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(16) %.fca.2.3.gep, i8 0, i64 16, i1 false)
  store i32 3, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 8
  store i32 1, ptr %.fca.2.8.gep, align 4
  %.fca.3.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 0
  store i32 0, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 1
  store i32 0, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 2
  store i32 3, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 3
  store i32 0, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 4
  store i32 1, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 5
  store i32 0, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 6
  store i32 0, ptr %.fca.3.6.gep, align 4
  %.fca.3.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 7
  store i32 8, ptr %.fca.3.7.gep, align 4
  %.fca.3.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 8
  store i32 0, ptr %.fca.3.8.gep, align 4
  %.fca.4.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 0
  store i32 9, ptr %.fca.4.0.gep, align 4
  %.fca.4.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 1
  store i32 0, ptr %.fca.4.1.gep, align 4
  %.fca.4.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 2
  store i32 0, ptr %.fca.4.2.gep, align 4
  %.fca.4.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 3
  store i32 8, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 4
  store i32 6, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 5
  store i32 3, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 6
  store i32 0, ptr %.fca.4.6.gep, align 4
  %.fca.4.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 7
  store i32 0, ptr %.fca.4.7.gep, align 4
  %.fca.4.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 8
  store i32 5, ptr %.fca.4.8.gep, align 4
  %.fca.5.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 0
  store i32 0, ptr %.fca.5.0.gep, align 4
  %.fca.5.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 1
  store i32 5, ptr %.fca.5.1.gep, align 4
  %.fca.5.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 2
  store i32 0, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 3
  store i32 0, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 4
  store i32 9, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 5
  store i32 0, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 6
  store i32 6, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 7
  store i32 0, ptr %.fca.5.7.gep, align 4
  %.fca.5.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 8
  store i32 0, ptr %.fca.5.8.gep, align 4
  %.fca.6.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 0
  store i32 1, ptr %.fca.6.0.gep, align 4
  %.fca.6.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 1
  store i32 3, ptr %.fca.6.1.gep, align 4
  %.fca.6.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 2
  %.fca.6.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 6
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(16) %.fca.6.2.gep, i8 0, i64 16, i1 false)
  store i32 2, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 7
  store i32 5, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 8
  %.fca.7.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 7
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(32) %.fca.6.8.gep, i8 0, i64 32, i1 false)
  store i32 7, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 8
  store i32 4, ptr %.fca.7.8.gep, align 4
  %.fca.8.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 0
  store i32 0, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 1
  store i32 0, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 2
  store i32 5, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 3
  store i32 2, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 4
  store i32 0, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 5
  store i32 6, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 6
  store i32 3, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 7
  store i32 0, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 8
  store i32 0, ptr %.fca.8.8.gep, align 4
  %1 = call fastcc i1 @_Z11solveSudokuPA9A9iii(ptr nonnull %grid, i32 0, i32 0) #4
  br i1 %1, label %if.then.L83, label %if.else.L83

if.then.L83:                                      ; preds = %0
  call fastcc void @_Z5printPA9A9i(ptr nonnull %grid) #4
  br label %if.exit.L83

if.else.L83:                                      ; preds = %0
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  br label %if.exit.L83

if.exit.L83:                                      ; preds = %if.else.L83, %if.then.L83
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { nofree nounwind }
attributes #1 = { nofree nosync nounwind memory(argmem: readwrite) }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #4 = { nounwind }
