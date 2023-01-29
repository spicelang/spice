; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__TreeNode__TreeNodeptr_int = type { ptr, i32 }

@anon.struct.0 = private unnamed_addr constant %__TreeNode__TreeNodeptr_int zeroinitializer
@printf.str.0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %rootNode = alloca %__TreeNode__TreeNodeptr_int, align 8
  %2 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode1 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %3 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode2 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %4 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode21 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %curNode = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.struct.0, i64 16, i1 false)
  store %__TreeNode__TreeNodeptr_int zeroinitializer, ptr %rootNode, align 8
  %5 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %2, i32 0, i32 0
  store ptr %rootNode, ptr %5, align 8
  %6 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %2, i32 0, i32 1
  store i32 1, ptr %6, align 4
  %7 = load %__TreeNode__TreeNodeptr_int, ptr %2, align 8
  store %__TreeNode__TreeNodeptr_int %7, ptr %childNode1, align 8
  %8 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %3, i32 0, i32 0
  store ptr %rootNode, ptr %8, align 8
  %9 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %3, i32 0, i32 1
  store i32 2, ptr %9, align 4
  %10 = load %__TreeNode__TreeNodeptr_int, ptr %3, align 8
  store %__TreeNode__TreeNodeptr_int %10, ptr %childNode2, align 8
  %11 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %4, i32 0, i32 0
  store ptr %childNode2, ptr %11, align 8
  %12 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %4, i32 0, i32 1
  store i32 3, ptr %12, align 4
  %13 = load %__TreeNode__TreeNodeptr_int, ptr %4, align 8
  store %__TreeNode__TreeNodeptr_int %13, ptr %childNode21, align 8
  store ptr %childNode21, ptr %curNode, align 8
  br label %while.head.L21

while.head.L21:                                   ; preds = %while.body.L21, %0
  %14 = load ptr, ptr %curNode, align 8
  %parent = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %14, i32 0, i32 0
  %15 = load ptr, ptr %parent, align 8
  %16 = icmp ne ptr %15, null
  br i1 %16, label %while.body.L21, label %while.exit.L21

while.body.L21:                                   ; preds = %while.head.L21
  %17 = load ptr, ptr %curNode, align 8
  %parent1 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %17, i32 0, i32 0
  %18 = load ptr, ptr %parent1, align 8
  store ptr %18, ptr %curNode, align 8
  br label %while.head.L21

while.exit.L21:                                   ; preds = %while.head.L21
  %19 = load ptr, ptr %curNode, align 8
  %value = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %19, i32 0, i32 1
  %20 = load i32, ptr %value, align 4
  %21 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %20)
  %22 = load i32, ptr %result, align 4
  ret i32 %22
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
