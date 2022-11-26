; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @_f__void__int__fib__int(i32 %0) unnamed_addr #0 {
entry.l1:
  %1 = icmp slt i32 %0, 3
  br i1 %1, label %common.ret, label %if.end.l2

common.ret.loopexit:                              ; preds = %if.end.l2
  %phi.bo = add i32 %5, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %entry.l1
  %accumulator.tr.lcssa = phi i32 [ 1, %entry.l1 ], [ %phi.bo, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.end.l2:                                        ; preds = %entry.l1, %if.end.l2
  %.tr9 = phi i32 [ %4, %if.end.l2 ], [ %0, %entry.l1 ]
  %accumulator.tr8 = phi i32 [ %5, %if.end.l2 ], [ 0, %entry.l1 ]
  %2 = add nsw i32 %.tr9, -1
  %3 = tail call fastcc i32 @_f__void__int__fib__int(i32 %2)
  %4 = add nsw i32 %.tr9, -2
  %5 = add i32 %3, %accumulator.tr8
  %6 = icmp ult i32 %.tr9, 5
  br i1 %6, label %common.ret.loopexit, label %if.end.l2
}

define i32 @main() local_unnamed_addr {
entry.l6:
  %0 = alloca {}, align 8
  %1 = alloca ptr, align 8
  %2 = call i32 @pthread_create(ptr nonnull %1, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %3 = load ptr, ptr %1, align 8
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr nonnull %4, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %6 = load ptr, ptr %4, align 8
  %7 = alloca ptr, align 8
  %8 = call i32 @pthread_create(ptr nonnull %7, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %9 = load ptr, ptr %7, align 8
  %10 = alloca ptr, align 8
  %11 = call i32 @pthread_create(ptr nonnull %10, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %12 = load ptr, ptr %10, align 8
  %13 = alloca ptr, align 8
  %14 = call i32 @pthread_create(ptr nonnull %13, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %15 = load ptr, ptr %13, align 8
  %16 = alloca ptr, align 8
  %17 = call i32 @pthread_create(ptr nonnull %16, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %18 = load ptr, ptr %16, align 8
  %19 = alloca ptr, align 8
  %20 = call i32 @pthread_create(ptr nonnull %19, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %21 = load ptr, ptr %19, align 8
  %22 = alloca ptr, align 8
  %23 = call i32 @pthread_create(ptr nonnull %22, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %24 = load ptr, ptr %22, align 8
  %puts = call i32 @puts(ptr nonnull @str)
  %25 = call i32 @pthread_join(ptr %3, ptr null)
  %26 = call i32 @pthread_join(ptr %6, ptr null)
  %27 = call i32 @pthread_join(ptr %9, ptr null)
  %28 = call i32 @pthread_join(ptr %12, ptr null)
  %29 = call i32 @pthread_join(ptr %15, ptr null)
  %30 = call i32 @pthread_join(ptr %18, ptr null)
  %31 = call i32 @pthread_join(ptr %21, ptr null)
  %32 = call i32 @pthread_join(ptr %24, ptr null)
  %33 = call i32 (ptr, ...) @printf(ptr nonnull @1)
  ret i32 0
}

; Function Attrs: nofree nounwind
define internal noalias ptr @_thread0(ptr nocapture readnone %0) #1 {
thread.entry.l10:
  %1 = tail call fastcc i32 @_f__void__int__fib__int(i32 46)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 %1)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

declare i32 @pthread_join(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
