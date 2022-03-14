; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@str = private unnamed_addr constant [24 x i8] c"Starting one thread ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1
@str.2 = private unnamed_addr constant [24 x i8] c"Hello from the thread 1\00", align 1
@str.3 = private unnamed_addr constant [24 x i8] c"Hello from the thread 2\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %0 = alloca {}, align 8
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @str, i64 0, i64 0))
  %1 = alloca i32, align 4
  %2 = bitcast {}* %0 to i8*
  %3 = call i32 @pthread_create(i32* nonnull %1, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %2)
  %4 = alloca i32, align 4
  %5 = call i32 @pthread_create(i32* nonnull %4, i8* null, i8* (i8*)* nonnull @_thread1, i8* nonnull %2)
  %6 = call i32 @usleep(i32 1000000)
  %puts3 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i64 0, i64 0))
  ret i32 0
}

define internal noalias i8* @_thread0(i8* nocapture readnone %0) {
entry:
  %1 = tail call i32 @usleep(i32 500000)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @str.2, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_create(i32*, i8*, i8* (i8*)*, i8*) local_unnamed_addr

define internal noalias i8* @_thread1(i8* nocapture readnone %0) {
entry:
  %1 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @str.3, i64 0, i64 0))
  ret i8* null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
