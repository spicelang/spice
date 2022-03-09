; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@str = private unnamed_addr constant [21 x i8] c"Starting threads ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %0 = alloca {}, align 8
  %1 = alloca i8, align 1
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([21 x i8], [21 x i8]* @str, i64 0, i64 0))
  %2 = bitcast {}* %0 to i8*
  %3 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 1)
  %4 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %5 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 2)
  %6 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %7 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 3)
  %8 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %9 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 4)
  %10 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %11 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 5)
  %12 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %13 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 6)
  %14 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %15 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 7)
  %16 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %17 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 8)
  %18 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %2)
  %19 = call i32 @usleep(i32 1000000)
  %puts7 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i64 0, i64 0))
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

define internal noalias i8* @_thread2(i8* nocapture readnone %0) {
entry:
  %1 = tail call i32 @usleep(i32 200000)
  ret i8* null
}

declare i32 @pthread_create(i8*, i8*, i8* (i8*)*, i8*) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
