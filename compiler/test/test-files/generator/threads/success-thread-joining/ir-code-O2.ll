; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@str = private unnamed_addr constant [9 x i8] c"Thread 1\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"Thread 1 finished\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"Thread 2\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Thread 2 finished\00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"Thread 3\00", align 1
@str.5 = private unnamed_addr constant [18 x i8] c"Thread 3 finished\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %0 = alloca {}, align 8
  %t1 = alloca i32, align 4
  %t3 = alloca i32, align 4
  %1 = alloca { i32*, i32* }, align 8
  %2 = alloca i32, align 4
  %3 = bitcast {}* %0 to i8*
  %4 = call i32 @pthread_create(i32* nonnull %2, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %3)
  %5 = load i32, i32* %2, align 4
  store i32 %5, i32* %t1, align 4
  %6 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i64 0, i32 0
  store i32* %t1, i32** %6, align 8
  %7 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i64 0, i32 1
  store i32* %t3, i32** %7, align 8
  %8 = alloca i32, align 4
  %9 = bitcast { i32*, i32* }* %1 to i8*
  %10 = call i32 @pthread_create(i32* nonnull %8, i8* null, i8* (i8*)* nonnull @_thread1, i8* nonnull %9)
  %11 = alloca i32, align 4
  %12 = call i32 @pthread_create(i32* nonnull %11, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %3)
  %13 = load i32, i32* %11, align 4
  store i32 %13, i32* %t3, align 4
  %14 = call i32 @usleep(i32 1000000)
  ret i32 0
}

define internal noalias i8* @_thread0(i8* nocapture readnone %0) {
entry:
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @str, i64 0, i64 0))
  %1 = tail call i32 @usleep(i32 300000)
  %puts3 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_create(i32*, i8*, i8* (i8*)*, i8*) local_unnamed_addr

define internal noalias i8* @_thread1(i8* nocapture readonly %0) {
entry:
  %1 = bitcast i8* %0 to i32**
  %2 = load i32*, i32** %1, align 8
  %3 = getelementptr inbounds i8, i8* %0, i64 8
  %4 = bitcast i8* %3 to i32**
  %5 = load i32*, i32** %4, align 8
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i64 0, i64 0))
  %6 = load i32, i32* %2, align 4
  %7 = zext i32 %6 to i64
  %8 = inttoptr i64 %7 to i8*
  %9 = tail call i32 @pthread_join(i8* %8, i8** null)
  %10 = load i32, i32* %5, align 4
  %11 = zext i32 %10 to i64
  %12 = inttoptr i64 %11 to i8*
  %13 = tail call i32 @pthread_join(i8* %12, i8** null)
  %puts1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.3, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_join(i8*, i8**) local_unnamed_addr

define internal noalias i8* @_thread2(i8* nocapture readnone %0) {
entry:
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i64 0, i64 0))
  %1 = tail call i32 @usleep(i32 200000)
  %puts3 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.5, i64 0, i64 0))
  ret i8* null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
