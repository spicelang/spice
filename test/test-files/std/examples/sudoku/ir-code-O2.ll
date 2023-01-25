; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@printf.str.2 = private unnamed_addr constant [23 x i8] c"No solution was found.\00", align 1

; Function Attrs: nofree nounwind
define private fastcc void @_p__void__void__print__intarrayarray() unnamed_addr #0 {
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
  br label %for.head.L5.preheader

for.head.L5.preheader:                            ; preds = %0, %for.head.L5.preheader
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %for.head.L5.preheader ]
  %1 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 0
  %2 = load i32, ptr %1, align 4
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %2)
  %4 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 1
  %5 = load i32, ptr %4, align 4
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %5)
  %7 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 2
  %8 = load i32, ptr %7, align 4
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %8)
  %10 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 3
  %11 = load i32, ptr %10, align 4
  %12 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %11)
  %13 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 4
  %14 = load i32, ptr %13, align 4
  %15 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %14)
  %16 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 5
  %17 = load i32, ptr %16, align 4
  %18 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %17)
  %19 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 6
  %20 = load i32, ptr %19, align 4
  %21 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %20)
  %22 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 7
  %23 = load i32, ptr %22, align 4
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %23)
  %25 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %indvars.iv, i64 8
  %26 = load i32, ptr %25, align 4
  %27 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %26)
  %putchar = tail call i32 @putchar(i32 10)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 9
  br i1 %exitcond.not, label %for.exit.L4, label %for.head.L5.preheader

for.exit.L4:                                      ; preds = %for.head.L5.preheader
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind readnone
define private fastcc i1 @_f__void__bool__solveSudoku__intarrayarray_int_int([9 x [9 x i32]] %0, i32 %1, i32 %2) unnamed_addr #1 {
land.exit.L45C8:
  %grid.i = alloca [9 x [9 x i32]], align 4
  %grid = alloca [9 x [9 x i32]], align 4
  %.fca.0.0.extract = extractvalue [9 x [9 x i32]] %0, 0, 0
  %.fca.0.1.extract = extractvalue [9 x [9 x i32]] %0, 0, 1
  %.fca.0.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 1
  %.fca.0.2.extract = extractvalue [9 x [9 x i32]] %0, 0, 2
  %.fca.0.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 2
  %.fca.0.3.extract = extractvalue [9 x [9 x i32]] %0, 0, 3
  %.fca.0.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 3
  %.fca.0.4.extract = extractvalue [9 x [9 x i32]] %0, 0, 4
  %.fca.0.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 4
  %.fca.0.5.extract = extractvalue [9 x [9 x i32]] %0, 0, 5
  %.fca.0.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 5
  %.fca.0.6.extract = extractvalue [9 x [9 x i32]] %0, 0, 6
  %.fca.0.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 6
  %.fca.0.7.extract = extractvalue [9 x [9 x i32]] %0, 0, 7
  %.fca.0.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 7
  %.fca.0.8.extract = extractvalue [9 x [9 x i32]] %0, 0, 8
  %.fca.0.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 0, i64 8
  %.fca.1.0.extract = extractvalue [9 x [9 x i32]] %0, 1, 0
  %.fca.1.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 0
  %.fca.1.1.extract = extractvalue [9 x [9 x i32]] %0, 1, 1
  %.fca.1.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 1
  %.fca.1.2.extract = extractvalue [9 x [9 x i32]] %0, 1, 2
  %.fca.1.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 2
  %.fca.1.3.extract = extractvalue [9 x [9 x i32]] %0, 1, 3
  %.fca.1.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 3
  %.fca.1.4.extract = extractvalue [9 x [9 x i32]] %0, 1, 4
  %.fca.1.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 4
  %.fca.1.5.extract = extractvalue [9 x [9 x i32]] %0, 1, 5
  %.fca.1.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 5
  %.fca.1.6.extract = extractvalue [9 x [9 x i32]] %0, 1, 6
  %.fca.1.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 6
  %.fca.1.7.extract = extractvalue [9 x [9 x i32]] %0, 1, 7
  %.fca.1.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 7
  %.fca.1.8.extract = extractvalue [9 x [9 x i32]] %0, 1, 8
  %.fca.1.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 1, i64 8
  %.fca.2.0.extract = extractvalue [9 x [9 x i32]] %0, 2, 0
  %.fca.2.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 0
  %.fca.2.1.extract = extractvalue [9 x [9 x i32]] %0, 2, 1
  %.fca.2.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 1
  %.fca.2.2.extract = extractvalue [9 x [9 x i32]] %0, 2, 2
  %.fca.2.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 2
  %.fca.2.3.extract = extractvalue [9 x [9 x i32]] %0, 2, 3
  %.fca.2.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 3
  %.fca.2.4.extract = extractvalue [9 x [9 x i32]] %0, 2, 4
  %.fca.2.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 4
  %.fca.2.5.extract = extractvalue [9 x [9 x i32]] %0, 2, 5
  %.fca.2.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 5
  %.fca.2.6.extract = extractvalue [9 x [9 x i32]] %0, 2, 6
  %.fca.2.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 6
  %.fca.2.7.extract = extractvalue [9 x [9 x i32]] %0, 2, 7
  %.fca.2.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 7
  %.fca.2.8.extract = extractvalue [9 x [9 x i32]] %0, 2, 8
  %.fca.2.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 2, i64 8
  %.fca.3.0.extract = extractvalue [9 x [9 x i32]] %0, 3, 0
  %.fca.3.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 0
  %.fca.3.1.extract = extractvalue [9 x [9 x i32]] %0, 3, 1
  %.fca.3.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 1
  %.fca.3.2.extract = extractvalue [9 x [9 x i32]] %0, 3, 2
  %.fca.3.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 2
  %.fca.3.3.extract = extractvalue [9 x [9 x i32]] %0, 3, 3
  %.fca.3.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 3
  %.fca.3.4.extract = extractvalue [9 x [9 x i32]] %0, 3, 4
  %.fca.3.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 4
  %.fca.3.5.extract = extractvalue [9 x [9 x i32]] %0, 3, 5
  %.fca.3.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 5
  %.fca.3.6.extract = extractvalue [9 x [9 x i32]] %0, 3, 6
  %.fca.3.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 6
  %.fca.3.7.extract = extractvalue [9 x [9 x i32]] %0, 3, 7
  %.fca.3.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 7
  %.fca.3.8.extract = extractvalue [9 x [9 x i32]] %0, 3, 8
  %.fca.3.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 3, i64 8
  %.fca.4.0.extract = extractvalue [9 x [9 x i32]] %0, 4, 0
  %.fca.4.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 0
  %.fca.4.1.extract = extractvalue [9 x [9 x i32]] %0, 4, 1
  %.fca.4.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 1
  %.fca.4.2.extract = extractvalue [9 x [9 x i32]] %0, 4, 2
  %.fca.4.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 2
  %.fca.4.3.extract = extractvalue [9 x [9 x i32]] %0, 4, 3
  %.fca.4.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 3
  %.fca.4.4.extract = extractvalue [9 x [9 x i32]] %0, 4, 4
  %.fca.4.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 4
  %.fca.4.5.extract = extractvalue [9 x [9 x i32]] %0, 4, 5
  %.fca.4.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 5
  %.fca.4.6.extract = extractvalue [9 x [9 x i32]] %0, 4, 6
  %.fca.4.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 6
  %.fca.4.7.extract = extractvalue [9 x [9 x i32]] %0, 4, 7
  %.fca.4.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 7
  %.fca.4.8.extract = extractvalue [9 x [9 x i32]] %0, 4, 8
  %.fca.4.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 4, i64 8
  %.fca.5.0.extract = extractvalue [9 x [9 x i32]] %0, 5, 0
  %.fca.5.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 0
  %.fca.5.1.extract = extractvalue [9 x [9 x i32]] %0, 5, 1
  %.fca.5.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 1
  %.fca.5.2.extract = extractvalue [9 x [9 x i32]] %0, 5, 2
  %.fca.5.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 2
  %.fca.5.3.extract = extractvalue [9 x [9 x i32]] %0, 5, 3
  %.fca.5.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 3
  %.fca.5.4.extract = extractvalue [9 x [9 x i32]] %0, 5, 4
  %.fca.5.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 4
  %.fca.5.5.extract = extractvalue [9 x [9 x i32]] %0, 5, 5
  %.fca.5.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 5
  %.fca.5.6.extract = extractvalue [9 x [9 x i32]] %0, 5, 6
  %.fca.5.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 6
  %.fca.5.7.extract = extractvalue [9 x [9 x i32]] %0, 5, 7
  %.fca.5.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 7
  %.fca.5.8.extract = extractvalue [9 x [9 x i32]] %0, 5, 8
  %.fca.5.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 5, i64 8
  %.fca.6.0.extract = extractvalue [9 x [9 x i32]] %0, 6, 0
  %.fca.6.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 0
  %.fca.6.1.extract = extractvalue [9 x [9 x i32]] %0, 6, 1
  %.fca.6.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 1
  %.fca.6.2.extract = extractvalue [9 x [9 x i32]] %0, 6, 2
  %.fca.6.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 2
  %.fca.6.3.extract = extractvalue [9 x [9 x i32]] %0, 6, 3
  %.fca.6.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 3
  %.fca.6.4.extract = extractvalue [9 x [9 x i32]] %0, 6, 4
  %.fca.6.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 4
  %.fca.6.5.extract = extractvalue [9 x [9 x i32]] %0, 6, 5
  %.fca.6.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 5
  %.fca.6.6.extract = extractvalue [9 x [9 x i32]] %0, 6, 6
  %.fca.6.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 6
  %.fca.6.7.extract = extractvalue [9 x [9 x i32]] %0, 6, 7
  %.fca.6.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 7
  %.fca.6.8.extract = extractvalue [9 x [9 x i32]] %0, 6, 8
  %.fca.6.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 6, i64 8
  %.fca.7.0.extract = extractvalue [9 x [9 x i32]] %0, 7, 0
  %.fca.7.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 0
  %.fca.7.1.extract = extractvalue [9 x [9 x i32]] %0, 7, 1
  %.fca.7.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 1
  %.fca.7.2.extract = extractvalue [9 x [9 x i32]] %0, 7, 2
  %.fca.7.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 2
  %.fca.7.3.extract = extractvalue [9 x [9 x i32]] %0, 7, 3
  %.fca.7.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 3
  %.fca.7.4.extract = extractvalue [9 x [9 x i32]] %0, 7, 4
  %.fca.7.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 4
  %.fca.7.5.extract = extractvalue [9 x [9 x i32]] %0, 7, 5
  %.fca.7.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 5
  %.fca.7.6.extract = extractvalue [9 x [9 x i32]] %0, 7, 6
  %.fca.7.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 6
  %.fca.7.7.extract = extractvalue [9 x [9 x i32]] %0, 7, 7
  %.fca.7.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 7
  %.fca.7.8.extract = extractvalue [9 x [9 x i32]] %0, 7, 8
  %.fca.7.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 7, i64 8
  %.fca.8.0.extract = extractvalue [9 x [9 x i32]] %0, 8, 0
  %.fca.8.0.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 0
  %.fca.8.1.extract = extractvalue [9 x [9 x i32]] %0, 8, 1
  %.fca.8.1.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 1
  %.fca.8.2.extract = extractvalue [9 x [9 x i32]] %0, 8, 2
  %.fca.8.2.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 2
  %.fca.8.3.extract = extractvalue [9 x [9 x i32]] %0, 8, 3
  %.fca.8.3.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 3
  %.fca.8.4.extract = extractvalue [9 x [9 x i32]] %0, 8, 4
  %.fca.8.4.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 4
  %.fca.8.5.extract = extractvalue [9 x [9 x i32]] %0, 8, 5
  %.fca.8.5.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 5
  %.fca.8.6.extract = extractvalue [9 x [9 x i32]] %0, 8, 6
  %.fca.8.6.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 6
  %.fca.8.7.extract = extractvalue [9 x [9 x i32]] %0, 8, 7
  %.fca.8.7.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 7
  %.fca.8.8.extract = extractvalue [9 x [9 x i32]] %0, 8, 8
  %.fca.8.8.gep = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 8, i64 8
  br label %tailrecurse

tailrecurse:                                      ; preds = %if.then.L52, %land.exit.L45C8
  %.tr587 = phi i32 [ %1, %land.exit.L45C8 ], [ %spec.select, %if.then.L52 ]
  %.tr588 = phi i32 [ %2, %land.exit.L45C8 ], [ %53, %if.then.L52 ]
  store i32 %.fca.0.0.extract, ptr %grid, align 4
  store i32 %.fca.0.1.extract, ptr %.fca.0.1.gep, align 4
  store i32 %.fca.0.2.extract, ptr %.fca.0.2.gep, align 4
  store i32 %.fca.0.3.extract, ptr %.fca.0.3.gep, align 4
  store i32 %.fca.0.4.extract, ptr %.fca.0.4.gep, align 4
  store i32 %.fca.0.5.extract, ptr %.fca.0.5.gep, align 4
  store i32 %.fca.0.6.extract, ptr %.fca.0.6.gep, align 4
  store i32 %.fca.0.7.extract, ptr %.fca.0.7.gep, align 4
  store i32 %.fca.0.8.extract, ptr %.fca.0.8.gep, align 4
  store i32 %.fca.1.0.extract, ptr %.fca.1.0.gep, align 4
  store i32 %.fca.1.1.extract, ptr %.fca.1.1.gep, align 4
  store i32 %.fca.1.2.extract, ptr %.fca.1.2.gep, align 4
  store i32 %.fca.1.3.extract, ptr %.fca.1.3.gep, align 4
  store i32 %.fca.1.4.extract, ptr %.fca.1.4.gep, align 4
  store i32 %.fca.1.5.extract, ptr %.fca.1.5.gep, align 4
  store i32 %.fca.1.6.extract, ptr %.fca.1.6.gep, align 4
  store i32 %.fca.1.7.extract, ptr %.fca.1.7.gep, align 4
  store i32 %.fca.1.8.extract, ptr %.fca.1.8.gep, align 4
  store i32 %.fca.2.0.extract, ptr %.fca.2.0.gep, align 4
  store i32 %.fca.2.1.extract, ptr %.fca.2.1.gep, align 4
  store i32 %.fca.2.2.extract, ptr %.fca.2.2.gep, align 4
  store i32 %.fca.2.3.extract, ptr %.fca.2.3.gep, align 4
  store i32 %.fca.2.4.extract, ptr %.fca.2.4.gep, align 4
  store i32 %.fca.2.5.extract, ptr %.fca.2.5.gep, align 4
  store i32 %.fca.2.6.extract, ptr %.fca.2.6.gep, align 4
  store i32 %.fca.2.7.extract, ptr %.fca.2.7.gep, align 4
  store i32 %.fca.2.8.extract, ptr %.fca.2.8.gep, align 4
  store i32 %.fca.3.0.extract, ptr %.fca.3.0.gep, align 4
  store i32 %.fca.3.1.extract, ptr %.fca.3.1.gep, align 4
  store i32 %.fca.3.2.extract, ptr %.fca.3.2.gep, align 4
  store i32 %.fca.3.3.extract, ptr %.fca.3.3.gep, align 4
  store i32 %.fca.3.4.extract, ptr %.fca.3.4.gep, align 4
  store i32 %.fca.3.5.extract, ptr %.fca.3.5.gep, align 4
  store i32 %.fca.3.6.extract, ptr %.fca.3.6.gep, align 4
  store i32 %.fca.3.7.extract, ptr %.fca.3.7.gep, align 4
  store i32 %.fca.3.8.extract, ptr %.fca.3.8.gep, align 4
  store i32 %.fca.4.0.extract, ptr %.fca.4.0.gep, align 4
  store i32 %.fca.4.1.extract, ptr %.fca.4.1.gep, align 4
  store i32 %.fca.4.2.extract, ptr %.fca.4.2.gep, align 4
  store i32 %.fca.4.3.extract, ptr %.fca.4.3.gep, align 4
  store i32 %.fca.4.4.extract, ptr %.fca.4.4.gep, align 4
  store i32 %.fca.4.5.extract, ptr %.fca.4.5.gep, align 4
  store i32 %.fca.4.6.extract, ptr %.fca.4.6.gep, align 4
  store i32 %.fca.4.7.extract, ptr %.fca.4.7.gep, align 4
  store i32 %.fca.4.8.extract, ptr %.fca.4.8.gep, align 4
  store i32 %.fca.5.0.extract, ptr %.fca.5.0.gep, align 4
  store i32 %.fca.5.1.extract, ptr %.fca.5.1.gep, align 4
  store i32 %.fca.5.2.extract, ptr %.fca.5.2.gep, align 4
  store i32 %.fca.5.3.extract, ptr %.fca.5.3.gep, align 4
  store i32 %.fca.5.4.extract, ptr %.fca.5.4.gep, align 4
  store i32 %.fca.5.5.extract, ptr %.fca.5.5.gep, align 4
  store i32 %.fca.5.6.extract, ptr %.fca.5.6.gep, align 4
  store i32 %.fca.5.7.extract, ptr %.fca.5.7.gep, align 4
  store i32 %.fca.5.8.extract, ptr %.fca.5.8.gep, align 4
  store i32 %.fca.6.0.extract, ptr %.fca.6.0.gep, align 4
  store i32 %.fca.6.1.extract, ptr %.fca.6.1.gep, align 4
  store i32 %.fca.6.2.extract, ptr %.fca.6.2.gep, align 4
  store i32 %.fca.6.3.extract, ptr %.fca.6.3.gep, align 4
  store i32 %.fca.6.4.extract, ptr %.fca.6.4.gep, align 4
  store i32 %.fca.6.5.extract, ptr %.fca.6.5.gep, align 4
  store i32 %.fca.6.6.extract, ptr %.fca.6.6.gep, align 4
  store i32 %.fca.6.7.extract, ptr %.fca.6.7.gep, align 4
  store i32 %.fca.6.8.extract, ptr %.fca.6.8.gep, align 4
  store i32 %.fca.7.0.extract, ptr %.fca.7.0.gep, align 4
  store i32 %.fca.7.1.extract, ptr %.fca.7.1.gep, align 4
  store i32 %.fca.7.2.extract, ptr %.fca.7.2.gep, align 4
  store i32 %.fca.7.3.extract, ptr %.fca.7.3.gep, align 4
  store i32 %.fca.7.4.extract, ptr %.fca.7.4.gep, align 4
  store i32 %.fca.7.5.extract, ptr %.fca.7.5.gep, align 4
  store i32 %.fca.7.6.extract, ptr %.fca.7.6.gep, align 4
  store i32 %.fca.7.7.extract, ptr %.fca.7.7.gep, align 4
  store i32 %.fca.7.8.extract, ptr %.fca.7.8.gep, align 4
  store i32 %.fca.8.0.extract, ptr %.fca.8.0.gep, align 4
  store i32 %.fca.8.1.extract, ptr %.fca.8.1.gep, align 4
  store i32 %.fca.8.2.extract, ptr %.fca.8.2.gep, align 4
  store i32 %.fca.8.3.extract, ptr %.fca.8.3.gep, align 4
  store i32 %.fca.8.4.extract, ptr %.fca.8.4.gep, align 4
  store i32 %.fca.8.5.extract, ptr %.fca.8.5.gep, align 4
  store i32 %.fca.8.6.extract, ptr %.fca.8.6.gep, align 4
  store i32 %.fca.8.7.extract, ptr %.fca.8.7.gep, align 4
  store i32 %.fca.8.8.extract, ptr %.fca.8.8.gep, align 4
  %3 = icmp eq i32 %.tr587, 8
  %4 = icmp eq i32 %.tr588, 9
  %land_phi = select i1 %3, i1 %4, i1 false
  br i1 %land_phi, label %common.ret, label %if.exit.L45

common.ret:                                       ; preds = %tailrecurse, %if.exit.L57, %if.then.L57
  %common.ret.op = phi i1 [ false, %if.exit.L57 ], [ true, %if.then.L57 ], [ true, %tailrecurse ]
  ret i1 %common.ret.op

if.exit.L45:                                      ; preds = %tailrecurse
  %5 = zext i1 %4 to i32
  %spec.select = add i32 %.tr587, %5
  %spec.select585 = select i1 %4, i32 0, i32 %.tr588
  %6 = sext i32 %spec.select to i64
  %7 = sext i32 %spec.select585 to i64
  %8 = getelementptr inbounds [9 x [9 x i32]], ptr %grid, i64 0, i64 %6, i64 %7
  %9 = load i32, ptr %8, align 4
  %10 = icmp sgt i32 %9, 0
  br i1 %10, label %if.then.L52, label %for.head.L56.preheader

for.head.L56.preheader:                           ; preds = %if.exit.L45
  %.fca.0.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 1
  %.fca.0.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 2
  %.fca.0.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 3
  %.fca.0.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 4
  %.fca.0.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 5
  %.fca.0.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 6
  %.fca.0.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 7
  %.fca.0.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 8
  %.fca.1.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 0
  %.fca.1.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 1
  %.fca.1.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 2
  %.fca.1.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 3
  %.fca.1.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 4
  %.fca.1.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 5
  %.fca.1.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 6
  %.fca.1.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 7
  %.fca.1.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 8
  %.fca.2.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 0
  %.fca.2.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 1
  %.fca.2.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 2
  %.fca.2.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 3
  %.fca.2.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 4
  %.fca.2.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 5
  %.fca.2.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 6
  %.fca.2.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 7
  %.fca.2.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 8
  %.fca.3.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 0
  %.fca.3.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 1
  %.fca.3.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 2
  %.fca.3.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 3
  %.fca.3.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 4
  %.fca.3.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 5
  %.fca.3.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 6
  %.fca.3.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 7
  %.fca.3.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 8
  %.fca.4.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 0
  %.fca.4.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 1
  %.fca.4.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 2
  %.fca.4.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 3
  %.fca.4.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 4
  %.fca.4.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 5
  %.fca.4.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 6
  %.fca.4.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 7
  %.fca.4.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 8
  %.fca.5.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 0
  %.fca.5.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 1
  %.fca.5.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 2
  %.fca.5.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 3
  %.fca.5.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 4
  %.fca.5.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 5
  %.fca.5.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 6
  %.fca.5.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 7
  %.fca.5.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 8
  %.fca.6.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 0
  %.fca.6.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 1
  %.fca.6.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 2
  %.fca.6.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 3
  %.fca.6.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 4
  %.fca.6.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 5
  %.fca.6.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 6
  %.fca.6.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 7
  %.fca.6.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 8
  %.fca.7.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 0
  %.fca.7.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 1
  %.fca.7.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 2
  %.fca.7.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 3
  %.fca.7.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 4
  %.fca.7.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 5
  %.fca.7.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 6
  %.fca.7.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 7
  %.fca.7.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 8
  %.fca.8.0.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 0
  %.fca.8.1.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 1
  %.fca.8.2.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 2
  %.fca.8.3.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 3
  %.fca.8.4.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 4
  %.fca.8.5.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 5
  %.fca.8.6.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 6
  %.fca.8.7.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 7
  %.fca.8.8.gep.i = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 8
  %11 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 0
  %12 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 1
  %13 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 2
  %14 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 3
  %15 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 4
  %16 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 5
  %17 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 6
  %18 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 7
  %19 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %6, i64 8
  %20 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 0, i64 %7
  %21 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 1, i64 %7
  %22 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 2, i64 %7
  %23 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 3, i64 %7
  %24 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 4, i64 %7
  %25 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 5, i64 %7
  %26 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 6, i64 %7
  %27 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 7, i64 %7
  %28 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 8, i64 %7
  %29 = srem i32 %spec.select, 3
  %30 = sub i32 %spec.select, %29
  %31 = srem i32 %spec.select585, 3
  %32 = sub i32 %spec.select585, %31
  %33 = sext i32 %30 to i64
  %34 = sext i32 %32 to i64
  %35 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %33, i64 %34
  %36 = add i32 %32, 1
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %33, i64 %37
  %39 = add i32 %32, 2
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %33, i64 %40
  %42 = add i32 %30, 1
  %43 = sext i32 %42 to i64
  %44 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %43, i64 %34
  %45 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %43, i64 %37
  %46 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %43, i64 %40
  %47 = add i32 %30, 2
  %48 = sext i32 %47 to i64
  %49 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %48, i64 %34
  %50 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %48, i64 %37
  %51 = getelementptr inbounds [9 x [9 x i32]], ptr %grid.i, i64 0, i64 %48, i64 %40
  %52 = add i32 %spec.select585, 1
  br label %for.body.L56

if.then.L52:                                      ; preds = %if.exit.L45
  %53 = add i32 %spec.select585, 1
  br label %tailrecurse

for.body.L56:                                     ; preds = %for.head.L56.preheader, %if.exit.L57
  %num.0672 = phi i32 [ 1, %for.head.L56.preheader ], [ %108, %if.exit.L57 ]
  %.fca.0.0.load100 = load i32, ptr %grid, align 4
  %.fca.0.1.load103 = load i32, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.load106 = load i32, ptr %.fca.0.2.gep, align 4
  %.fca.0.3.load109 = load i32, ptr %.fca.0.3.gep, align 4
  %.fca.0.4.load112 = load i32, ptr %.fca.0.4.gep, align 4
  %.fca.0.5.load115 = load i32, ptr %.fca.0.5.gep, align 4
  %.fca.0.6.load118 = load i32, ptr %.fca.0.6.gep, align 4
  %.fca.0.7.load121 = load i32, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.load124 = load i32, ptr %.fca.0.8.gep, align 4
  %.fca.1.0.load127 = load i32, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.load130 = load i32, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.load133 = load i32, ptr %.fca.1.2.gep, align 4
  %.fca.1.3.load136 = load i32, ptr %.fca.1.3.gep, align 4
  %.fca.1.4.load139 = load i32, ptr %.fca.1.4.gep, align 4
  %.fca.1.5.load142 = load i32, ptr %.fca.1.5.gep, align 4
  %.fca.1.6.load145 = load i32, ptr %.fca.1.6.gep, align 4
  %.fca.1.7.load148 = load i32, ptr %.fca.1.7.gep, align 4
  %.fca.1.8.load151 = load i32, ptr %.fca.1.8.gep, align 4
  %.fca.2.0.load154 = load i32, ptr %.fca.2.0.gep, align 4
  %.fca.2.1.load157 = load i32, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.load160 = load i32, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.load163 = load i32, ptr %.fca.2.3.gep, align 4
  %.fca.2.4.load166 = load i32, ptr %.fca.2.4.gep, align 4
  %.fca.2.5.load169 = load i32, ptr %.fca.2.5.gep, align 4
  %.fca.2.6.load172 = load i32, ptr %.fca.2.6.gep, align 4
  %.fca.2.7.load175 = load i32, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.load178 = load i32, ptr %.fca.2.8.gep, align 4
  %.fca.3.0.load181 = load i32, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.load184 = load i32, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.load187 = load i32, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.load190 = load i32, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.load193 = load i32, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.load196 = load i32, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.load199 = load i32, ptr %.fca.3.6.gep, align 4
  %.fca.3.7.load202 = load i32, ptr %.fca.3.7.gep, align 4
  %.fca.3.8.load205 = load i32, ptr %.fca.3.8.gep, align 4
  %.fca.4.0.load208 = load i32, ptr %.fca.4.0.gep, align 4
  %.fca.4.1.load211 = load i32, ptr %.fca.4.1.gep, align 4
  %.fca.4.2.load214 = load i32, ptr %.fca.4.2.gep, align 4
  %.fca.4.3.load217 = load i32, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.load220 = load i32, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.load223 = load i32, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.load226 = load i32, ptr %.fca.4.6.gep, align 4
  %.fca.4.7.load229 = load i32, ptr %.fca.4.7.gep, align 4
  %.fca.4.8.load232 = load i32, ptr %.fca.4.8.gep, align 4
  %.fca.5.0.load235 = load i32, ptr %.fca.5.0.gep, align 4
  %.fca.5.1.load238 = load i32, ptr %.fca.5.1.gep, align 4
  %.fca.5.2.load241 = load i32, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.load244 = load i32, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.load247 = load i32, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.load250 = load i32, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.load253 = load i32, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.load256 = load i32, ptr %.fca.5.7.gep, align 4
  %.fca.5.8.load259 = load i32, ptr %.fca.5.8.gep, align 4
  %.fca.6.0.load262 = load i32, ptr %.fca.6.0.gep, align 4
  %.fca.6.1.load265 = load i32, ptr %.fca.6.1.gep, align 4
  %.fca.6.2.load268 = load i32, ptr %.fca.6.2.gep, align 4
  %.fca.6.3.load271 = load i32, ptr %.fca.6.3.gep, align 4
  %.fca.6.4.load274 = load i32, ptr %.fca.6.4.gep, align 4
  %.fca.6.5.load277 = load i32, ptr %.fca.6.5.gep, align 4
  %.fca.6.6.load280 = load i32, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.load283 = load i32, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.load286 = load i32, ptr %.fca.6.8.gep, align 4
  %.fca.7.0.load289 = load i32, ptr %.fca.7.0.gep, align 4
  %.fca.7.1.load292 = load i32, ptr %.fca.7.1.gep, align 4
  %.fca.7.2.load295 = load i32, ptr %.fca.7.2.gep, align 4
  %.fca.7.3.load298 = load i32, ptr %.fca.7.3.gep, align 4
  %.fca.7.4.load301 = load i32, ptr %.fca.7.4.gep, align 4
  %.fca.7.5.load304 = load i32, ptr %.fca.7.5.gep, align 4
  %.fca.7.6.load307 = load i32, ptr %.fca.7.6.gep, align 4
  %.fca.7.7.load310 = load i32, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.load313 = load i32, ptr %.fca.7.8.gep, align 4
  %.fca.8.0.load316 = load i32, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.load319 = load i32, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.load322 = load i32, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.load325 = load i32, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.load328 = load i32, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.load331 = load i32, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.load334 = load i32, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.load337 = load i32, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.load340 = load i32, ptr %.fca.8.8.gep, align 4
  call void @llvm.lifetime.start.p0(i64 324, ptr nonnull %grid.i)
  store i32 %.fca.0.0.load100, ptr %grid.i, align 4
  store i32 %.fca.0.1.load103, ptr %.fca.0.1.gep.i, align 4
  store i32 %.fca.0.2.load106, ptr %.fca.0.2.gep.i, align 4
  store i32 %.fca.0.3.load109, ptr %.fca.0.3.gep.i, align 4
  store i32 %.fca.0.4.load112, ptr %.fca.0.4.gep.i, align 4
  store i32 %.fca.0.5.load115, ptr %.fca.0.5.gep.i, align 4
  store i32 %.fca.0.6.load118, ptr %.fca.0.6.gep.i, align 4
  store i32 %.fca.0.7.load121, ptr %.fca.0.7.gep.i, align 4
  store i32 %.fca.0.8.load124, ptr %.fca.0.8.gep.i, align 4
  store i32 %.fca.1.0.load127, ptr %.fca.1.0.gep.i, align 4
  store i32 %.fca.1.1.load130, ptr %.fca.1.1.gep.i, align 4
  store i32 %.fca.1.2.load133, ptr %.fca.1.2.gep.i, align 4
  store i32 %.fca.1.3.load136, ptr %.fca.1.3.gep.i, align 4
  store i32 %.fca.1.4.load139, ptr %.fca.1.4.gep.i, align 4
  store i32 %.fca.1.5.load142, ptr %.fca.1.5.gep.i, align 4
  store i32 %.fca.1.6.load145, ptr %.fca.1.6.gep.i, align 4
  store i32 %.fca.1.7.load148, ptr %.fca.1.7.gep.i, align 4
  store i32 %.fca.1.8.load151, ptr %.fca.1.8.gep.i, align 4
  store i32 %.fca.2.0.load154, ptr %.fca.2.0.gep.i, align 4
  store i32 %.fca.2.1.load157, ptr %.fca.2.1.gep.i, align 4
  store i32 %.fca.2.2.load160, ptr %.fca.2.2.gep.i, align 4
  store i32 %.fca.2.3.load163, ptr %.fca.2.3.gep.i, align 4
  store i32 %.fca.2.4.load166, ptr %.fca.2.4.gep.i, align 4
  store i32 %.fca.2.5.load169, ptr %.fca.2.5.gep.i, align 4
  store i32 %.fca.2.6.load172, ptr %.fca.2.6.gep.i, align 4
  store i32 %.fca.2.7.load175, ptr %.fca.2.7.gep.i, align 4
  store i32 %.fca.2.8.load178, ptr %.fca.2.8.gep.i, align 4
  store i32 %.fca.3.0.load181, ptr %.fca.3.0.gep.i, align 4
  store i32 %.fca.3.1.load184, ptr %.fca.3.1.gep.i, align 4
  store i32 %.fca.3.2.load187, ptr %.fca.3.2.gep.i, align 4
  store i32 %.fca.3.3.load190, ptr %.fca.3.3.gep.i, align 4
  store i32 %.fca.3.4.load193, ptr %.fca.3.4.gep.i, align 4
  store i32 %.fca.3.5.load196, ptr %.fca.3.5.gep.i, align 4
  store i32 %.fca.3.6.load199, ptr %.fca.3.6.gep.i, align 4
  store i32 %.fca.3.7.load202, ptr %.fca.3.7.gep.i, align 4
  store i32 %.fca.3.8.load205, ptr %.fca.3.8.gep.i, align 4
  store i32 %.fca.4.0.load208, ptr %.fca.4.0.gep.i, align 4
  store i32 %.fca.4.1.load211, ptr %.fca.4.1.gep.i, align 4
  store i32 %.fca.4.2.load214, ptr %.fca.4.2.gep.i, align 4
  store i32 %.fca.4.3.load217, ptr %.fca.4.3.gep.i, align 4
  store i32 %.fca.4.4.load220, ptr %.fca.4.4.gep.i, align 4
  store i32 %.fca.4.5.load223, ptr %.fca.4.5.gep.i, align 4
  store i32 %.fca.4.6.load226, ptr %.fca.4.6.gep.i, align 4
  store i32 %.fca.4.7.load229, ptr %.fca.4.7.gep.i, align 4
  store i32 %.fca.4.8.load232, ptr %.fca.4.8.gep.i, align 4
  store i32 %.fca.5.0.load235, ptr %.fca.5.0.gep.i, align 4
  store i32 %.fca.5.1.load238, ptr %.fca.5.1.gep.i, align 4
  store i32 %.fca.5.2.load241, ptr %.fca.5.2.gep.i, align 4
  store i32 %.fca.5.3.load244, ptr %.fca.5.3.gep.i, align 4
  store i32 %.fca.5.4.load247, ptr %.fca.5.4.gep.i, align 4
  store i32 %.fca.5.5.load250, ptr %.fca.5.5.gep.i, align 4
  store i32 %.fca.5.6.load253, ptr %.fca.5.6.gep.i, align 4
  store i32 %.fca.5.7.load256, ptr %.fca.5.7.gep.i, align 4
  store i32 %.fca.5.8.load259, ptr %.fca.5.8.gep.i, align 4
  store i32 %.fca.6.0.load262, ptr %.fca.6.0.gep.i, align 4
  store i32 %.fca.6.1.load265, ptr %.fca.6.1.gep.i, align 4
  store i32 %.fca.6.2.load268, ptr %.fca.6.2.gep.i, align 4
  store i32 %.fca.6.3.load271, ptr %.fca.6.3.gep.i, align 4
  store i32 %.fca.6.4.load274, ptr %.fca.6.4.gep.i, align 4
  store i32 %.fca.6.5.load277, ptr %.fca.6.5.gep.i, align 4
  store i32 %.fca.6.6.load280, ptr %.fca.6.6.gep.i, align 4
  store i32 %.fca.6.7.load283, ptr %.fca.6.7.gep.i, align 4
  store i32 %.fca.6.8.load286, ptr %.fca.6.8.gep.i, align 4
  store i32 %.fca.7.0.load289, ptr %.fca.7.0.gep.i, align 4
  store i32 %.fca.7.1.load292, ptr %.fca.7.1.gep.i, align 4
  store i32 %.fca.7.2.load295, ptr %.fca.7.2.gep.i, align 4
  store i32 %.fca.7.3.load298, ptr %.fca.7.3.gep.i, align 4
  store i32 %.fca.7.4.load301, ptr %.fca.7.4.gep.i, align 4
  store i32 %.fca.7.5.load304, ptr %.fca.7.5.gep.i, align 4
  store i32 %.fca.7.6.load307, ptr %.fca.7.6.gep.i, align 4
  store i32 %.fca.7.7.load310, ptr %.fca.7.7.gep.i, align 4
  store i32 %.fca.7.8.load313, ptr %.fca.7.8.gep.i, align 4
  store i32 %.fca.8.0.load316, ptr %.fca.8.0.gep.i, align 4
  store i32 %.fca.8.1.load319, ptr %.fca.8.1.gep.i, align 4
  store i32 %.fca.8.2.load322, ptr %.fca.8.2.gep.i, align 4
  store i32 %.fca.8.3.load325, ptr %.fca.8.3.gep.i, align 4
  store i32 %.fca.8.4.load328, ptr %.fca.8.4.gep.i, align 4
  store i32 %.fca.8.5.load331, ptr %.fca.8.5.gep.i, align 4
  store i32 %.fca.8.6.load334, ptr %.fca.8.6.gep.i, align 4
  store i32 %.fca.8.7.load337, ptr %.fca.8.7.gep.i, align 4
  store i32 %.fca.8.8.load340, ptr %.fca.8.8.gep.i, align 4
  %54 = load i32, ptr %11, align 4
  %55 = icmp eq i32 %54, %num.0672
  br i1 %55, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.i

for.head.L14.i:                                   ; preds = %for.body.L56
  %56 = load i32, ptr %12, align 4
  %57 = icmp eq i32 %56, %num.0672
  br i1 %57, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.1.i

for.head.L14.1.i:                                 ; preds = %for.head.L14.i
  %58 = load i32, ptr %13, align 4
  %59 = icmp eq i32 %58, %num.0672
  br i1 %59, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.2.i

for.head.L14.2.i:                                 ; preds = %for.head.L14.1.i
  %60 = load i32, ptr %14, align 4
  %61 = icmp eq i32 %60, %num.0672
  br i1 %61, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.3.i

for.head.L14.3.i:                                 ; preds = %for.head.L14.2.i
  %62 = load i32, ptr %15, align 4
  %63 = icmp eq i32 %62, %num.0672
  br i1 %63, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.4.i

for.head.L14.4.i:                                 ; preds = %for.head.L14.3.i
  %64 = load i32, ptr %16, align 4
  %65 = icmp eq i32 %64, %num.0672
  br i1 %65, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.5.i

for.head.L14.5.i:                                 ; preds = %for.head.L14.4.i
  %66 = load i32, ptr %17, align 4
  %67 = icmp eq i32 %66, %num.0672
  br i1 %67, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.6.i

for.head.L14.6.i:                                 ; preds = %for.head.L14.5.i
  %68 = load i32, ptr %18, align 4
  %69 = icmp eq i32 %68, %num.0672
  br i1 %69, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.7.i

for.head.L14.7.i:                                 ; preds = %for.head.L14.6.i
  %70 = load i32, ptr %19, align 4
  %71 = icmp eq i32 %70, %num.0672
  br i1 %71, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L14.8.i

for.head.L14.8.i:                                 ; preds = %for.head.L14.7.i
  %72 = load i32, ptr %20, align 4
  %73 = icmp eq i32 %72, %num.0672
  br i1 %73, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.i

for.head.L21.i:                                   ; preds = %for.head.L14.8.i
  %74 = load i32, ptr %21, align 4
  %75 = icmp eq i32 %74, %num.0672
  br i1 %75, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.1.i

for.head.L21.1.i:                                 ; preds = %for.head.L21.i
  %76 = load i32, ptr %22, align 4
  %77 = icmp eq i32 %76, %num.0672
  br i1 %77, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.2.i

for.head.L21.2.i:                                 ; preds = %for.head.L21.1.i
  %78 = load i32, ptr %23, align 4
  %79 = icmp eq i32 %78, %num.0672
  br i1 %79, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.3.i

for.head.L21.3.i:                                 ; preds = %for.head.L21.2.i
  %80 = load i32, ptr %24, align 4
  %81 = icmp eq i32 %80, %num.0672
  br i1 %81, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.4.i

for.head.L21.4.i:                                 ; preds = %for.head.L21.3.i
  %82 = load i32, ptr %25, align 4
  %83 = icmp eq i32 %82, %num.0672
  br i1 %83, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.5.i

for.head.L21.5.i:                                 ; preds = %for.head.L21.4.i
  %84 = load i32, ptr %26, align 4
  %85 = icmp eq i32 %84, %num.0672
  br i1 %85, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.6.i

for.head.L21.6.i:                                 ; preds = %for.head.L21.5.i
  %86 = load i32, ptr %27, align 4
  %87 = icmp eq i32 %86, %num.0672
  br i1 %87, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.7.i

for.head.L21.7.i:                                 ; preds = %for.head.L21.6.i
  %88 = load i32, ptr %28, align 4
  %89 = icmp eq i32 %88, %num.0672
  br i1 %89, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L21.8.i

for.head.L21.8.i:                                 ; preds = %for.head.L21.7.i
  %90 = load i32, ptr %35, align 4
  %91 = icmp eq i32 %90, %num.0672
  br i1 %91, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.i

for.head.L32.i:                                   ; preds = %for.head.L21.8.i
  %92 = load i32, ptr %38, align 4
  %93 = icmp eq i32 %92, %num.0672
  br i1 %93, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.1.i

for.head.L32.1.i:                                 ; preds = %for.head.L32.i
  %94 = load i32, ptr %41, align 4
  %95 = icmp eq i32 %94, %num.0672
  br i1 %95, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.2.i

for.head.L32.2.i:                                 ; preds = %for.head.L32.1.i
  %96 = load i32, ptr %44, align 4
  %97 = icmp eq i32 %96, %num.0672
  br i1 %97, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.131.i

for.head.L32.131.i:                               ; preds = %for.head.L32.2.i
  %98 = load i32, ptr %45, align 4
  %99 = icmp eq i32 %98, %num.0672
  br i1 %99, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.1.1.i

for.head.L32.1.1.i:                               ; preds = %for.head.L32.131.i
  %100 = load i32, ptr %46, align 4
  %101 = icmp eq i32 %100, %num.0672
  br i1 %101, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.2.1.i

for.head.L32.2.1.i:                               ; preds = %for.head.L32.1.1.i
  %102 = load i32, ptr %49, align 4
  %103 = icmp eq i32 %102, %num.0672
  br i1 %103, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %for.head.L32.232.i

for.head.L32.232.i:                               ; preds = %for.head.L32.2.1.i
  %104 = load i32, ptr %50, align 4
  %105 = icmp eq i32 %104, %num.0672
  br i1 %105, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, label %_f__void__bool__isSafe__intarrayarray_int_int_int.exit

_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread: ; preds = %for.head.L32.232.i, %for.head.L32.2.1.i, %for.head.L32.1.1.i, %for.head.L32.131.i, %for.head.L32.2.i, %for.head.L32.1.i, %for.head.L32.i, %for.head.L21.8.i, %for.head.L21.7.i, %for.head.L21.6.i, %for.head.L21.5.i, %for.head.L21.4.i, %for.head.L21.3.i, %for.head.L21.2.i, %for.head.L21.1.i, %for.head.L21.i, %for.head.L14.8.i, %for.head.L14.7.i, %for.head.L14.6.i, %for.head.L14.5.i, %for.head.L14.4.i, %for.head.L14.3.i, %for.head.L14.2.i, %for.head.L14.1.i, %for.head.L14.i, %for.body.L56
  call void @llvm.lifetime.end.p0(i64 324, ptr nonnull %grid.i)
  br label %if.exit.L57

_f__void__bool__isSafe__intarrayarray_int_int_int.exit: ; preds = %for.head.L32.232.i
  %106 = load i32, ptr %51, align 4
  %.not = icmp eq i32 %106, %num.0672
  call void @llvm.lifetime.end.p0(i64 324, ptr nonnull %grid.i)
  br i1 %.not, label %if.exit.L57, label %if.then.L57

if.then.L57:                                      ; preds = %_f__void__bool__isSafe__intarrayarray_int_int_int.exit
  store i32 %num.0672, ptr %8, align 4
  %.fca.0.0.load343 = load i32, ptr %grid, align 4
  %.fca.0.0.insert344 = insertvalue [9 x [9 x i32]] poison, i32 %.fca.0.0.load343, 0, 0
  %.fca.0.1.load346 = load i32, ptr %.fca.0.1.gep, align 4
  %.fca.0.1.insert347 = insertvalue [9 x [9 x i32]] %.fca.0.0.insert344, i32 %.fca.0.1.load346, 0, 1
  %.fca.0.2.load349 = load i32, ptr %.fca.0.2.gep, align 4
  %.fca.0.2.insert350 = insertvalue [9 x [9 x i32]] %.fca.0.1.insert347, i32 %.fca.0.2.load349, 0, 2
  %.fca.0.3.load352 = load i32, ptr %.fca.0.3.gep, align 4
  %.fca.0.3.insert353 = insertvalue [9 x [9 x i32]] %.fca.0.2.insert350, i32 %.fca.0.3.load352, 0, 3
  %.fca.0.4.load355 = load i32, ptr %.fca.0.4.gep, align 4
  %.fca.0.4.insert356 = insertvalue [9 x [9 x i32]] %.fca.0.3.insert353, i32 %.fca.0.4.load355, 0, 4
  %.fca.0.5.load358 = load i32, ptr %.fca.0.5.gep, align 4
  %.fca.0.5.insert359 = insertvalue [9 x [9 x i32]] %.fca.0.4.insert356, i32 %.fca.0.5.load358, 0, 5
  %.fca.0.6.load361 = load i32, ptr %.fca.0.6.gep, align 4
  %.fca.0.6.insert362 = insertvalue [9 x [9 x i32]] %.fca.0.5.insert359, i32 %.fca.0.6.load361, 0, 6
  %.fca.0.7.load364 = load i32, ptr %.fca.0.7.gep, align 4
  %.fca.0.7.insert365 = insertvalue [9 x [9 x i32]] %.fca.0.6.insert362, i32 %.fca.0.7.load364, 0, 7
  %.fca.0.8.load367 = load i32, ptr %.fca.0.8.gep, align 4
  %.fca.0.8.insert368 = insertvalue [9 x [9 x i32]] %.fca.0.7.insert365, i32 %.fca.0.8.load367, 0, 8
  %.fca.1.0.load370 = load i32, ptr %.fca.1.0.gep, align 4
  %.fca.1.0.insert371 = insertvalue [9 x [9 x i32]] %.fca.0.8.insert368, i32 %.fca.1.0.load370, 1, 0
  %.fca.1.1.load373 = load i32, ptr %.fca.1.1.gep, align 4
  %.fca.1.1.insert374 = insertvalue [9 x [9 x i32]] %.fca.1.0.insert371, i32 %.fca.1.1.load373, 1, 1
  %.fca.1.2.load376 = load i32, ptr %.fca.1.2.gep, align 4
  %.fca.1.2.insert377 = insertvalue [9 x [9 x i32]] %.fca.1.1.insert374, i32 %.fca.1.2.load376, 1, 2
  %.fca.1.3.load379 = load i32, ptr %.fca.1.3.gep, align 4
  %.fca.1.3.insert380 = insertvalue [9 x [9 x i32]] %.fca.1.2.insert377, i32 %.fca.1.3.load379, 1, 3
  %.fca.1.4.load382 = load i32, ptr %.fca.1.4.gep, align 4
  %.fca.1.4.insert383 = insertvalue [9 x [9 x i32]] %.fca.1.3.insert380, i32 %.fca.1.4.load382, 1, 4
  %.fca.1.5.load385 = load i32, ptr %.fca.1.5.gep, align 4
  %.fca.1.5.insert386 = insertvalue [9 x [9 x i32]] %.fca.1.4.insert383, i32 %.fca.1.5.load385, 1, 5
  %.fca.1.6.load388 = load i32, ptr %.fca.1.6.gep, align 4
  %.fca.1.6.insert389 = insertvalue [9 x [9 x i32]] %.fca.1.5.insert386, i32 %.fca.1.6.load388, 1, 6
  %.fca.1.7.load391 = load i32, ptr %.fca.1.7.gep, align 4
  %.fca.1.7.insert392 = insertvalue [9 x [9 x i32]] %.fca.1.6.insert389, i32 %.fca.1.7.load391, 1, 7
  %.fca.1.8.load394 = load i32, ptr %.fca.1.8.gep, align 4
  %.fca.1.8.insert395 = insertvalue [9 x [9 x i32]] %.fca.1.7.insert392, i32 %.fca.1.8.load394, 1, 8
  %.fca.2.0.load397 = load i32, ptr %.fca.2.0.gep, align 4
  %.fca.2.0.insert398 = insertvalue [9 x [9 x i32]] %.fca.1.8.insert395, i32 %.fca.2.0.load397, 2, 0
  %.fca.2.1.load400 = load i32, ptr %.fca.2.1.gep, align 4
  %.fca.2.1.insert401 = insertvalue [9 x [9 x i32]] %.fca.2.0.insert398, i32 %.fca.2.1.load400, 2, 1
  %.fca.2.2.load403 = load i32, ptr %.fca.2.2.gep, align 4
  %.fca.2.2.insert404 = insertvalue [9 x [9 x i32]] %.fca.2.1.insert401, i32 %.fca.2.2.load403, 2, 2
  %.fca.2.3.load406 = load i32, ptr %.fca.2.3.gep, align 4
  %.fca.2.3.insert407 = insertvalue [9 x [9 x i32]] %.fca.2.2.insert404, i32 %.fca.2.3.load406, 2, 3
  %.fca.2.4.load409 = load i32, ptr %.fca.2.4.gep, align 4
  %.fca.2.4.insert410 = insertvalue [9 x [9 x i32]] %.fca.2.3.insert407, i32 %.fca.2.4.load409, 2, 4
  %.fca.2.5.load412 = load i32, ptr %.fca.2.5.gep, align 4
  %.fca.2.5.insert413 = insertvalue [9 x [9 x i32]] %.fca.2.4.insert410, i32 %.fca.2.5.load412, 2, 5
  %.fca.2.6.load415 = load i32, ptr %.fca.2.6.gep, align 4
  %.fca.2.6.insert416 = insertvalue [9 x [9 x i32]] %.fca.2.5.insert413, i32 %.fca.2.6.load415, 2, 6
  %.fca.2.7.load418 = load i32, ptr %.fca.2.7.gep, align 4
  %.fca.2.7.insert419 = insertvalue [9 x [9 x i32]] %.fca.2.6.insert416, i32 %.fca.2.7.load418, 2, 7
  %.fca.2.8.load421 = load i32, ptr %.fca.2.8.gep, align 4
  %.fca.2.8.insert422 = insertvalue [9 x [9 x i32]] %.fca.2.7.insert419, i32 %.fca.2.8.load421, 2, 8
  %.fca.3.0.load424 = load i32, ptr %.fca.3.0.gep, align 4
  %.fca.3.0.insert425 = insertvalue [9 x [9 x i32]] %.fca.2.8.insert422, i32 %.fca.3.0.load424, 3, 0
  %.fca.3.1.load427 = load i32, ptr %.fca.3.1.gep, align 4
  %.fca.3.1.insert428 = insertvalue [9 x [9 x i32]] %.fca.3.0.insert425, i32 %.fca.3.1.load427, 3, 1
  %.fca.3.2.load430 = load i32, ptr %.fca.3.2.gep, align 4
  %.fca.3.2.insert431 = insertvalue [9 x [9 x i32]] %.fca.3.1.insert428, i32 %.fca.3.2.load430, 3, 2
  %.fca.3.3.load433 = load i32, ptr %.fca.3.3.gep, align 4
  %.fca.3.3.insert434 = insertvalue [9 x [9 x i32]] %.fca.3.2.insert431, i32 %.fca.3.3.load433, 3, 3
  %.fca.3.4.load436 = load i32, ptr %.fca.3.4.gep, align 4
  %.fca.3.4.insert437 = insertvalue [9 x [9 x i32]] %.fca.3.3.insert434, i32 %.fca.3.4.load436, 3, 4
  %.fca.3.5.load439 = load i32, ptr %.fca.3.5.gep, align 4
  %.fca.3.5.insert440 = insertvalue [9 x [9 x i32]] %.fca.3.4.insert437, i32 %.fca.3.5.load439, 3, 5
  %.fca.3.6.load442 = load i32, ptr %.fca.3.6.gep, align 4
  %.fca.3.6.insert443 = insertvalue [9 x [9 x i32]] %.fca.3.5.insert440, i32 %.fca.3.6.load442, 3, 6
  %.fca.3.7.load445 = load i32, ptr %.fca.3.7.gep, align 4
  %.fca.3.7.insert446 = insertvalue [9 x [9 x i32]] %.fca.3.6.insert443, i32 %.fca.3.7.load445, 3, 7
  %.fca.3.8.load448 = load i32, ptr %.fca.3.8.gep, align 4
  %.fca.3.8.insert449 = insertvalue [9 x [9 x i32]] %.fca.3.7.insert446, i32 %.fca.3.8.load448, 3, 8
  %.fca.4.0.load451 = load i32, ptr %.fca.4.0.gep, align 4
  %.fca.4.0.insert452 = insertvalue [9 x [9 x i32]] %.fca.3.8.insert449, i32 %.fca.4.0.load451, 4, 0
  %.fca.4.1.load454 = load i32, ptr %.fca.4.1.gep, align 4
  %.fca.4.1.insert455 = insertvalue [9 x [9 x i32]] %.fca.4.0.insert452, i32 %.fca.4.1.load454, 4, 1
  %.fca.4.2.load457 = load i32, ptr %.fca.4.2.gep, align 4
  %.fca.4.2.insert458 = insertvalue [9 x [9 x i32]] %.fca.4.1.insert455, i32 %.fca.4.2.load457, 4, 2
  %.fca.4.3.load460 = load i32, ptr %.fca.4.3.gep, align 4
  %.fca.4.3.insert461 = insertvalue [9 x [9 x i32]] %.fca.4.2.insert458, i32 %.fca.4.3.load460, 4, 3
  %.fca.4.4.load463 = load i32, ptr %.fca.4.4.gep, align 4
  %.fca.4.4.insert464 = insertvalue [9 x [9 x i32]] %.fca.4.3.insert461, i32 %.fca.4.4.load463, 4, 4
  %.fca.4.5.load466 = load i32, ptr %.fca.4.5.gep, align 4
  %.fca.4.5.insert467 = insertvalue [9 x [9 x i32]] %.fca.4.4.insert464, i32 %.fca.4.5.load466, 4, 5
  %.fca.4.6.load469 = load i32, ptr %.fca.4.6.gep, align 4
  %.fca.4.6.insert470 = insertvalue [9 x [9 x i32]] %.fca.4.5.insert467, i32 %.fca.4.6.load469, 4, 6
  %.fca.4.7.load472 = load i32, ptr %.fca.4.7.gep, align 4
  %.fca.4.7.insert473 = insertvalue [9 x [9 x i32]] %.fca.4.6.insert470, i32 %.fca.4.7.load472, 4, 7
  %.fca.4.8.load475 = load i32, ptr %.fca.4.8.gep, align 4
  %.fca.4.8.insert476 = insertvalue [9 x [9 x i32]] %.fca.4.7.insert473, i32 %.fca.4.8.load475, 4, 8
  %.fca.5.0.load478 = load i32, ptr %.fca.5.0.gep, align 4
  %.fca.5.0.insert479 = insertvalue [9 x [9 x i32]] %.fca.4.8.insert476, i32 %.fca.5.0.load478, 5, 0
  %.fca.5.1.load481 = load i32, ptr %.fca.5.1.gep, align 4
  %.fca.5.1.insert482 = insertvalue [9 x [9 x i32]] %.fca.5.0.insert479, i32 %.fca.5.1.load481, 5, 1
  %.fca.5.2.load484 = load i32, ptr %.fca.5.2.gep, align 4
  %.fca.5.2.insert485 = insertvalue [9 x [9 x i32]] %.fca.5.1.insert482, i32 %.fca.5.2.load484, 5, 2
  %.fca.5.3.load487 = load i32, ptr %.fca.5.3.gep, align 4
  %.fca.5.3.insert488 = insertvalue [9 x [9 x i32]] %.fca.5.2.insert485, i32 %.fca.5.3.load487, 5, 3
  %.fca.5.4.load490 = load i32, ptr %.fca.5.4.gep, align 4
  %.fca.5.4.insert491 = insertvalue [9 x [9 x i32]] %.fca.5.3.insert488, i32 %.fca.5.4.load490, 5, 4
  %.fca.5.5.load493 = load i32, ptr %.fca.5.5.gep, align 4
  %.fca.5.5.insert494 = insertvalue [9 x [9 x i32]] %.fca.5.4.insert491, i32 %.fca.5.5.load493, 5, 5
  %.fca.5.6.load496 = load i32, ptr %.fca.5.6.gep, align 4
  %.fca.5.6.insert497 = insertvalue [9 x [9 x i32]] %.fca.5.5.insert494, i32 %.fca.5.6.load496, 5, 6
  %.fca.5.7.load499 = load i32, ptr %.fca.5.7.gep, align 4
  %.fca.5.7.insert500 = insertvalue [9 x [9 x i32]] %.fca.5.6.insert497, i32 %.fca.5.7.load499, 5, 7
  %.fca.5.8.load502 = load i32, ptr %.fca.5.8.gep, align 4
  %.fca.5.8.insert503 = insertvalue [9 x [9 x i32]] %.fca.5.7.insert500, i32 %.fca.5.8.load502, 5, 8
  %.fca.6.0.load505 = load i32, ptr %.fca.6.0.gep, align 4
  %.fca.6.0.insert506 = insertvalue [9 x [9 x i32]] %.fca.5.8.insert503, i32 %.fca.6.0.load505, 6, 0
  %.fca.6.1.load508 = load i32, ptr %.fca.6.1.gep, align 4
  %.fca.6.1.insert509 = insertvalue [9 x [9 x i32]] %.fca.6.0.insert506, i32 %.fca.6.1.load508, 6, 1
  %.fca.6.2.load511 = load i32, ptr %.fca.6.2.gep, align 4
  %.fca.6.2.insert512 = insertvalue [9 x [9 x i32]] %.fca.6.1.insert509, i32 %.fca.6.2.load511, 6, 2
  %.fca.6.3.load514 = load i32, ptr %.fca.6.3.gep, align 4
  %.fca.6.3.insert515 = insertvalue [9 x [9 x i32]] %.fca.6.2.insert512, i32 %.fca.6.3.load514, 6, 3
  %.fca.6.4.load517 = load i32, ptr %.fca.6.4.gep, align 4
  %.fca.6.4.insert518 = insertvalue [9 x [9 x i32]] %.fca.6.3.insert515, i32 %.fca.6.4.load517, 6, 4
  %.fca.6.5.load520 = load i32, ptr %.fca.6.5.gep, align 4
  %.fca.6.5.insert521 = insertvalue [9 x [9 x i32]] %.fca.6.4.insert518, i32 %.fca.6.5.load520, 6, 5
  %.fca.6.6.load523 = load i32, ptr %.fca.6.6.gep, align 4
  %.fca.6.6.insert524 = insertvalue [9 x [9 x i32]] %.fca.6.5.insert521, i32 %.fca.6.6.load523, 6, 6
  %.fca.6.7.load526 = load i32, ptr %.fca.6.7.gep, align 4
  %.fca.6.7.insert527 = insertvalue [9 x [9 x i32]] %.fca.6.6.insert524, i32 %.fca.6.7.load526, 6, 7
  %.fca.6.8.load529 = load i32, ptr %.fca.6.8.gep, align 4
  %.fca.6.8.insert530 = insertvalue [9 x [9 x i32]] %.fca.6.7.insert527, i32 %.fca.6.8.load529, 6, 8
  %.fca.7.0.load532 = load i32, ptr %.fca.7.0.gep, align 4
  %.fca.7.0.insert533 = insertvalue [9 x [9 x i32]] %.fca.6.8.insert530, i32 %.fca.7.0.load532, 7, 0
  %.fca.7.1.load535 = load i32, ptr %.fca.7.1.gep, align 4
  %.fca.7.1.insert536 = insertvalue [9 x [9 x i32]] %.fca.7.0.insert533, i32 %.fca.7.1.load535, 7, 1
  %.fca.7.2.load538 = load i32, ptr %.fca.7.2.gep, align 4
  %.fca.7.2.insert539 = insertvalue [9 x [9 x i32]] %.fca.7.1.insert536, i32 %.fca.7.2.load538, 7, 2
  %.fca.7.3.load541 = load i32, ptr %.fca.7.3.gep, align 4
  %.fca.7.3.insert542 = insertvalue [9 x [9 x i32]] %.fca.7.2.insert539, i32 %.fca.7.3.load541, 7, 3
  %.fca.7.4.load544 = load i32, ptr %.fca.7.4.gep, align 4
  %.fca.7.4.insert545 = insertvalue [9 x [9 x i32]] %.fca.7.3.insert542, i32 %.fca.7.4.load544, 7, 4
  %.fca.7.5.load547 = load i32, ptr %.fca.7.5.gep, align 4
  %.fca.7.5.insert548 = insertvalue [9 x [9 x i32]] %.fca.7.4.insert545, i32 %.fca.7.5.load547, 7, 5
  %.fca.7.6.load550 = load i32, ptr %.fca.7.6.gep, align 4
  %.fca.7.6.insert551 = insertvalue [9 x [9 x i32]] %.fca.7.5.insert548, i32 %.fca.7.6.load550, 7, 6
  %.fca.7.7.load553 = load i32, ptr %.fca.7.7.gep, align 4
  %.fca.7.7.insert554 = insertvalue [9 x [9 x i32]] %.fca.7.6.insert551, i32 %.fca.7.7.load553, 7, 7
  %.fca.7.8.load556 = load i32, ptr %.fca.7.8.gep, align 4
  %.fca.7.8.insert557 = insertvalue [9 x [9 x i32]] %.fca.7.7.insert554, i32 %.fca.7.8.load556, 7, 8
  %.fca.8.0.load559 = load i32, ptr %.fca.8.0.gep, align 4
  %.fca.8.0.insert560 = insertvalue [9 x [9 x i32]] %.fca.7.8.insert557, i32 %.fca.8.0.load559, 8, 0
  %.fca.8.1.load562 = load i32, ptr %.fca.8.1.gep, align 4
  %.fca.8.1.insert563 = insertvalue [9 x [9 x i32]] %.fca.8.0.insert560, i32 %.fca.8.1.load562, 8, 1
  %.fca.8.2.load565 = load i32, ptr %.fca.8.2.gep, align 4
  %.fca.8.2.insert566 = insertvalue [9 x [9 x i32]] %.fca.8.1.insert563, i32 %.fca.8.2.load565, 8, 2
  %.fca.8.3.load568 = load i32, ptr %.fca.8.3.gep, align 4
  %.fca.8.3.insert569 = insertvalue [9 x [9 x i32]] %.fca.8.2.insert566, i32 %.fca.8.3.load568, 8, 3
  %.fca.8.4.load571 = load i32, ptr %.fca.8.4.gep, align 4
  %.fca.8.4.insert572 = insertvalue [9 x [9 x i32]] %.fca.8.3.insert569, i32 %.fca.8.4.load571, 8, 4
  %.fca.8.5.load574 = load i32, ptr %.fca.8.5.gep, align 4
  %.fca.8.5.insert575 = insertvalue [9 x [9 x i32]] %.fca.8.4.insert572, i32 %.fca.8.5.load574, 8, 5
  %.fca.8.6.load577 = load i32, ptr %.fca.8.6.gep, align 4
  %.fca.8.6.insert578 = insertvalue [9 x [9 x i32]] %.fca.8.5.insert575, i32 %.fca.8.6.load577, 8, 6
  %.fca.8.7.load580 = load i32, ptr %.fca.8.7.gep, align 4
  %.fca.8.7.insert581 = insertvalue [9 x [9 x i32]] %.fca.8.6.insert578, i32 %.fca.8.7.load580, 8, 7
  %.fca.8.8.load583 = load i32, ptr %.fca.8.8.gep, align 4
  %.fca.8.8.insert584 = insertvalue [9 x [9 x i32]] %.fca.8.7.insert581, i32 %.fca.8.8.load583, 8, 8
  %107 = tail call fastcc i1 @_f__void__bool__solveSudoku__intarrayarray_int_int([9 x [9 x i32]] %.fca.8.8.insert584, i32 %spec.select, i32 %52)
  br i1 %107, label %common.ret, label %if.exit.L57

if.exit.L57:                                      ; preds = %_f__void__bool__isSafe__intarrayarray_int_int_int.exit.thread, %if.then.L57, %_f__void__bool__isSafe__intarrayarray_int_int_int.exit
  store i32 0, ptr %8, align 4
  %108 = add nuw nsw i32 %num.0672, 1
  %exitcond.not = icmp eq i32 %108, 10
  br i1 %exitcond.not, label %common.ret, label %for.body.L56
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
  %1 = tail call fastcc i1 @_f__void__bool__solveSudoku__intarrayarray_int_int([9 x [9 x i32]] [[9 x i32] [i32 3, i32 0, i32 6, i32 5, i32 0, i32 8, i32 4, i32 0, i32 0], [9 x i32] [i32 5, i32 2, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0], [9 x i32] [i32 0, i32 8, i32 7, i32 0, i32 0, i32 0, i32 0, i32 3, i32 1], [9 x i32] [i32 0, i32 0, i32 3, i32 0, i32 1, i32 0, i32 0, i32 8, i32 0], [9 x i32] [i32 9, i32 0, i32 0, i32 8, i32 6, i32 3, i32 0, i32 0, i32 5], [9 x i32] [i32 0, i32 5, i32 0, i32 0, i32 9, i32 0, i32 6, i32 0, i32 0], [9 x i32] [i32 1, i32 3, i32 0, i32 0, i32 0, i32 0, i32 2, i32 5, i32 0], [9 x i32] [i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 7, i32 4], [9 x i32] [i32 0, i32 0, i32 5, i32 2, i32 0, i32 6, i32 3, i32 0, i32 0]], i32 0, i32 0) #5
  br i1 %1, label %if.then.L83, label %if.else.L83

if.then.L83:                                      ; preds = %0
  tail call fastcc void @_p__void__void__print__intarrayarray() #5
  br label %if.exit.L83

if.else.L83:                                      ; preds = %0
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.2)
  br label %if.exit.L83

if.exit.L83:                                      ; preds = %if.else.L83, %if.then.L83
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #3

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #3

; Function Attrs: argmemonly nocallback nofree nounwind willreturn writeonly
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

attributes #0 = { nofree nounwind }
attributes #1 = { nofree nosync nounwind readnone }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { argmemonly nocallback nofree nosync nounwind willreturn }
attributes #4 = { argmemonly nocallback nofree nounwind willreturn writeonly }
attributes #5 = { nounwind }
