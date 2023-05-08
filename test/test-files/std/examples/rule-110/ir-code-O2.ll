; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@anon.string.0 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@anon.string.1 = private unnamed_addr constant [2 x i8] c" \00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
for.exit.L8:
  %board = alloca [80 x i32], align 4
  %.fca.1.gep = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 1
  %.fca.78.gep = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 78
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(320) %board, i8 0, i64 320, i1 false)
  store i32 1, ptr %.fca.78.gep, align 4
  br label %for.head.L15.preheader

for.head.L15.preheader:                           ; preds = %for.exit.L8, %for.tail.L14
  %i1.016 = phi i32 [ 0, %for.exit.L8 ], [ %22, %for.tail.L14 ]
  br label %for.body.L15

for.body.L15:                                     ; preds = %for.head.L15.preheader, %for.body.L15
  %indvars.iv = phi i64 [ 0, %for.head.L15.preheader ], [ %indvars.iv.next, %for.body.L15 ]
  %0 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv
  %1 = load i32, ptr %0, align 4
  %2 = icmp eq i32 %1, 1
  %3 = select i1 %2, ptr @anon.string.0, ptr @anon.string.1
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr nonnull %3)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 80
  br i1 %exitcond.not, label %for.exit.L15, label %for.body.L15

for.exit.L15:                                     ; preds = %for.body.L15
  %putchar = tail call i32 @putchar(i32 10)
  %5 = load i32, ptr %.fca.1.gep, align 4
  br label %for.body.L21

for.body.L21:                                     ; preds = %for.body.L21, %for.exit.L15
  %indvars.iv19 = phi i64 [ 1, %for.exit.L15 ], [ %indvars.iv.next20.1, %for.body.L21 ]
  %pattern.014 = phi i32 [ %5, %for.exit.L15 ], [ %18, %for.body.L21 ]
  %6 = shl i32 %pattern.014, 1
  %7 = and i32 %6, 6
  %indvars.iv.next20 = add nuw nsw i64 %indvars.iv19, 1
  %8 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20
  %9 = load i32, ptr %8, align 4
  %10 = or i32 %9, %7
  %11 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv19
  %12 = lshr i32 110, %10
  %13 = and i32 %12, 1
  store i32 %13, ptr %11, align 4
  %14 = shl i32 %10, 1
  %15 = and i32 %14, 6
  %indvars.iv.next20.1 = add nuw nsw i64 %indvars.iv19, 2
  %16 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.1
  %17 = load i32, ptr %16, align 4
  %18 = or i32 %17, %15
  %19 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20
  %20 = lshr i32 110, %18
  %21 = and i32 %20, 1
  store i32 %21, ptr %19, align 4
  %exitcond22.not.1 = icmp eq i64 %indvars.iv.next20.1, 79
  br i1 %exitcond22.not.1, label %for.tail.L14, label %for.body.L21

for.tail.L14:                                     ; preds = %for.body.L21
  %22 = add nuw nsw i32 %i1.016, 1
  %exitcond23.not = icmp eq i32 %22, 78
  br i1 %exitcond23.not, label %for.exit.L14, label %for.head.L15.preheader

for.exit.L14:                                     ; preds = %for.tail.L14
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
