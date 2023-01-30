; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_f__void__int__fib__int(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 3
  br i1 %2, label %common.ret, label %if.exit.L2

common.ret.loopexit:                              ; preds = %if.exit.L2
  %3 = add i32 %7, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %3, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %1, %if.exit.L2
  %.tr4 = phi i32 [ %6, %if.exit.L2 ], [ %0, %1 ]
  %accumulator.tr3 = phi i32 [ %7, %if.exit.L2 ], [ 0, %1 ]
  %4 = add nsw i32 %.tr4, -1
  %5 = tail call fastcc i32 @_f__void__int__fib__int(i32 %4)
  %6 = add nsw i32 %.tr4, -2
  %7 = add i32 %5, %accumulator.tr3
  %8 = icmp ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L2
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
  %9 = load ptr, ptr %7, align 8
  %10 = alloca ptr, align 8
  %11 = call i32 @pthread_create(ptr nonnull %10, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %12 = load ptr, ptr %10, align 8
  %13 = alloca ptr, align 8
  %14 = call i32 @pthread_create(ptr nonnull %13, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %15 = load ptr, ptr %13, align 8
  %16 = alloca ptr, align 8
  %17 = call i32 @pthread_create(ptr nonnull %16, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %18 = load ptr, ptr %16, align 8
  %19 = alloca ptr, align 8
  %20 = call i32 @pthread_create(ptr nonnull %19, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %21 = load ptr, ptr %19, align 8
  %22 = alloca ptr, align 8
  %23 = call i32 @pthread_create(ptr nonnull %22, ptr null, ptr nonnull @_thread0, ptr nonnull %0) #3
  %24 = load ptr, ptr %22, align 8
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %25 = call i32 @pthread_join(ptr %3, ptr null) #3
  %26 = call i32 @pthread_join(ptr %6, ptr null) #3
  %27 = call i32 @pthread_join(ptr %9, ptr null) #3
  %28 = call i32 @pthread_join(ptr %12, ptr null) #3
  %29 = call i32 @pthread_join(ptr %15, ptr null) #3
  %30 = call i32 @pthread_join(ptr %18, ptr null) #3
  %31 = call i32 @pthread_join(ptr %21, ptr null) #3
  %32 = call i32 @pthread_join(ptr %24, ptr null) #3
  %33 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
define private noalias ptr @_thread0(ptr nocapture readnone %0) #2 {
  %2 = tail call fastcc i32 @_f__void__int__fib__int(i32 30)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %2)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

declare i32 @pthread_join(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }
