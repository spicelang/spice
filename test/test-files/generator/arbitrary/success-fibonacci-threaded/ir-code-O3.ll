; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @_f__void__fib__int(i32 %0) unnamed_addr #0 {
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
  %3 = tail call fastcc i32 @_f__void__fib__int(i32 %2)
  %4 = add nsw i32 %.tr9, -2
  %5 = add i32 %3, %accumulator.tr8
  %6 = icmp ult i32 %.tr9, 5
  br i1 %6, label %common.ret.loopexit, label %if.end.l2
}

define i32 @main() local_unnamed_addr {
entry.l6:
  %0 = alloca {}, align 8
  %1 = bitcast {}* %0 to i8*
  %2 = alloca i8*, align 8
  %3 = call i32 @pthread_create(i8** nonnull %2, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %4 = load i8*, i8** %2, align 8
  %5 = alloca i8*, align 8
  %6 = call i32 @pthread_create(i8** nonnull %5, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %7 = load i8*, i8** %5, align 8
  %8 = alloca i8*, align 8
  %9 = call i32 @pthread_create(i8** nonnull %8, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %10 = load i8*, i8** %8, align 8
  %11 = alloca i8*, align 8
  %12 = call i32 @pthread_create(i8** nonnull %11, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %13 = load i8*, i8** %11, align 8
  %14 = alloca i8*, align 8
  %15 = call i32 @pthread_create(i8** nonnull %14, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %16 = load i8*, i8** %14, align 8
  %17 = alloca i8*, align 8
  %18 = call i32 @pthread_create(i8** nonnull %17, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %19 = load i8*, i8** %17, align 8
  %20 = alloca i8*, align 8
  %21 = call i32 @pthread_create(i8** nonnull %20, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %22 = load i8*, i8** %20, align 8
  %23 = alloca i8*, align 8
  %24 = call i32 @pthread_create(i8** nonnull %23, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %1)
  %25 = load i8*, i8** %23, align 8
  %puts = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([45 x i8], [45 x i8]* @str, i64 0, i64 0))
  %26 = call i32 @pthread_join(i8* %4, i8** null)
  %27 = call i32 @pthread_join(i8* %7, i8** null)
  %28 = call i32 @pthread_join(i8* %10, i8** null)
  %29 = call i32 @pthread_join(i8* %13, i8** null)
  %30 = call i32 @pthread_join(i8* %16, i8** null)
  %31 = call i32 @pthread_join(i8* %19, i8** null)
  %32 = call i32 @pthread_join(i8* %22, i8** null)
  %33 = call i32 @pthread_join(i8* %25, i8** null)
  %34 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([17 x i8], [17 x i8]* @1, i64 0, i64 0))
  ret i32 0
}

; Function Attrs: nofree nounwind
define internal noalias i8* @_thread0(i8* nocapture readnone %0) #1 {
thread.entry.l10:
  %1 = tail call fastcc i32 @_f__void__fib__int(i32 46)
  %2 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @0, i64 0, i64 0), i32 %1)
  ret i8* null
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #1

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*) local_unnamed_addr

declare i32 @pthread_join(i8*, i8**) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
