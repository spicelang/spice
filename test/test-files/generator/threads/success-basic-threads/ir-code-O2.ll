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
entry.l3:
  %0 = alloca {}, align 8
  %puts = tail call i32 @puts(ptr nonnull @str)
  %1 = alloca ptr, align 8
  %2 = call i32 @pthread_create(ptr nonnull %1, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %3 = alloca ptr, align 8
  %4 = call i32 @pthread_create(ptr nonnull %3, ptr null, ptr nonnull @_thread1, ptr nonnull %0)
  %5 = call i32 @usleep(i32 1000000)
  %puts1 = call i32 @puts(ptr nonnull @str.1)
  ret i32 0
}

define internal noalias ptr @_thread0(ptr nocapture readnone %0) {
thread.entry.l5:
  %1 = tail call i32 @usleep(i32 500000)
  %puts = tail call i32 @puts(ptr nonnull @str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

define internal noalias ptr @_thread1(ptr nocapture readnone %0) {
thread.entry.l9:
  %1 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(ptr nonnull @str.3)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
