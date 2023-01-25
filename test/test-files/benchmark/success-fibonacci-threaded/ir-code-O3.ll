; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define private fastcc i32 @_f__void__int__fib__int(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 3
  br i1 %2, label %common.ret, label %if.exit.L2

common.ret.loopexit:                              ; preds = %if.exit.L2
  %phi.bo = add i32 %6, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %phi.bo, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %1, %if.exit.L2
  %.tr4 = phi i32 [ %5, %if.exit.L2 ], [ %0, %1 ]
  %accumulator.tr3 = phi i32 [ %6, %if.exit.L2 ], [ 0, %1 ]
  %3 = add nsw i32 %.tr4, -1
  %4 = tail call fastcc i32 @_f__void__int__fib__int(i32 %3)
  %5 = add nsw i32 %.tr4, -2
  %6 = add i32 %4, %accumulator.tr3
  %7 = icmp ult i32 %.tr4, 5
  br i1 %7, label %common.ret.loopexit, label %if.exit.L2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
for.body.L9:
  %0 = alloca {}, align 8
  %1 = alloca ptr, align 8
  %2 = call i32 @pthread_create(ptr nonnull %1, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %3 = load ptr, ptr %1, align 8
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr nonnull %4, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %6 = load ptr, ptr %4, align 8
  %7 = alloca ptr, align 8
  %8 = call i32 @pthread_create(ptr nonnull %7, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr nonnull %9, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %11 = load ptr, ptr %9, align 8
  %12 = alloca ptr, align 8
  %13 = call i32 @pthread_create(ptr nonnull %12, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %14 = alloca ptr, align 8
  %15 = call i32 @pthread_create(ptr nonnull %14, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %16 = alloca ptr, align 8
  %17 = call i32 @pthread_create(ptr nonnull %16, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %18 = load ptr, ptr %16, align 8
  %19 = alloca ptr, align 8
  %20 = call i32 @pthread_create(ptr nonnull %19, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %puts = call i32 @puts(ptr nonnull @str)
  %21 = call i32 @pthread_join(ptr %3, ptr null) #3
  %22 = call i32 @pthread_join(ptr %6, ptr null) #3
  %23 = call i32 @pthread_join(ptr %11, ptr null) #3
  %24 = call i32 @pthread_join(ptr %18, ptr null) #3
  %25 = call i32 @pthread_join(ptr , ptr null) #3
  %26 = call i32 @pthread_join(ptr poison, ptr null) #3
  %27 = call i32 @pthread_join(ptr poison, ptr null) #3
  %28 = call i32 @pthread_join(ptr poison, ptr null) #3
  %29 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
define private noalias ptr @_thread0(ptr nocapture readnone %0) #2 {
  %2 = tail call fastcc i32 @_f__void__int__fib__int(i32 46)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %2)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

declare i32 @pthread_join(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }