; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [26 x i8] c"Hello from the thread %d\0A\00", align 1
@str = private unnamed_addr constant [21 x i8] c"Starting threads ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %0 = alloca { i32 }, align 8
  %1 = alloca i8, align 1
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([21 x i8], [21 x i8]* @str, i64 0, i64 0))
  %2 = getelementptr inbounds { i32 }, { i32 }* %0, i64 0, i32 0
  %3 = bitcast { i32 }* %0 to i8*
  store i32 0, i32* %2, align 8
  %4 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 1, i32* %2, align 8
  %5 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 2, i32* %2, align 8
  %6 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 3, i32* %2, align 8
  %7 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 4, i32* %2, align 8
  %8 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 5, i32* %2, align 8
  %9 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 6, i32* %2, align 8
  %10 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 7, i32* %2, align 8
  %11 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 8, i32* %2, align 8
  %12 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  store i32 9, i32* %2, align 8
  %13 = call i32 @pthread_create(i8* nonnull %1, i8* null, i8* (i8*)* nonnull @1, i8* nonnull %3)
  %14 = call i32 @usleep(i32 1000000)
  %puts8 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i64 0, i64 0))
  ret i32 0
}

define internal noalias i8* @1(i8* %0) {
entry:
  %1 = tail call i32 @usleep(i32 200000)
  %2 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @0, i64 0, i64 0), i8* %0)
  ret i8* null
}

declare i32 @pthread_create(i8*, i8*, i8* (i8*)*, i8*) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
