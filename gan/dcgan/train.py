# -*- coding: utf-8 -*-

import tensorflow as tf
import os

from read_data import *
from utils import *
from ops import *
from model import *
from model import BATCH_SIZE


def train():

    # 设置 global_step ，用来记录训练过程中的 step        
    global_step = tf.Variable(0, name = 'global_step', trainable = False)
    # 训练过程中的日志保存文件
    train_dir = '/home/jackie/workspace/gan/dcgan/logs'

    # 放置三个 placeholder，y 表示约束条件，images 表示送入判别器的图片，
    # z 表示随机噪声
    y= tf.placeholder(tf.float32, [BATCH_SIZE, 10], name='y')
    images = tf.placeholder(tf.float32, [64, 28, 28, 1], name='real_images')
    z = tf.placeholder(tf.float32, [None, 100], name='z')

    # 由生成器生成图像 G
    G = generator(z, y)
    # 真实图像送入判别器
    D, D_logits  = discriminator(images, y)
    # 采样器采样图像
    samples = sampler(z, y)
    # 生成图像送入判别器
    D_, D_logits_ = discriminator(G, y, reuse = True)
    
    # 损失计算
    d_loss_real = tf.reduce_mean(
        tf.nn.sigmoid_cross_entropy_with_logits(D_logits, tf.ones_like(D)))
    d_loss_fake = tf.reduce_mean(
        tf.nn.sigmoid_cross_entropy_with_logits(D_logits_, tf.zeros_like(D_)))
    d_loss = d_loss_real + d_loss_fake
    g_loss = tf.reduce_mean(
        tf.nn.sigmoid_cross_entropy_with_logits(D_logits_, tf.ones_like(D_)))

    # 总结操作
    z_sum = tf.histogram_summary("z", z)
    d_sum = tf.histogram_summary("d", D)
    d__sum = tf.histogram_summary("d_", D_)
    G_sum = tf.image_summary("G", G)

    d_loss_real_sum = tf.scalar_summary("d_loss_real", d_loss_real)
    d_loss_fake_sum = tf.scalar_summary("d_loss_fake", d_loss_fake)
    d_loss_sum = tf.scalar_summary("d_loss", d_loss)                                                
    g_loss_sum = tf.scalar_summary("g_loss", g_loss)
    
    # 合并各自的总结
    g_sum = tf.merge_summary([z_sum, d__sum, G_sum, d_loss_fake_sum, g_loss_sum])
    d_sum = tf.merge_summary([z_sum, d_sum, d_loss_real_sum, d_loss_sum])

    # 生成器和判别器要更新的变量，用于 tf.train.Optimizer 的 var_list
    t_vars = tf.trainable_variables()
    d_vars = [var for var in t_vars if 'd_' in var.name]
    g_vars = [var for var in t_vars if 'g_' in var.name]

    saver = tf.train.Saver()
    
    # 优化算法采用 Adam
    d_optim = tf.train.AdamOptimizer(0.0002, beta1 = 0.5) \
                .minimize(d_loss, var_list = d_vars, global_step = global_step)
    g_optim = tf.train.AdamOptimizer(0.0002, beta1 = 0.5) \
                .minimize(g_loss, var_list = g_vars, global_step = global_step)
        
    
    os.environ['CUDA_VISIBLE_DEVICES'] = str(0)
    config = tf.ConfigProto()
    config.gpu_options.per_process_gpu_memory_fraction = 0.2
    sess = tf.InteractiveSession(config=config)

    init = tf.initialize_all_variables()   
    writer = tf.train.SummaryWriter(train_dir, sess.graph)
    
    # 这个自己理解吧
    data_x, data_y = read_data()
    sample_z = np.random.uniform(-1, 1, size=(BATCH_SIZE, 100))
#    sample_images = data_x[0: 64]
    sample_labels = data_y[0: 64]
    sess.run(init)    
    
    # 循环 25 个 epoch 训练网络
    for epoch in range(25):
        batch_idxs = 1093
        for idx in range(batch_idxs):        
            batch_images = data_x[idx*64: (idx+1)*64]
            batch_labels = data_y[idx*64: (idx+1)*64]
            batch_z = np.random.uniform(-1, 1, size=(BATCH_SIZE, 100))            
            
            # 更新 D 的参数
            _, summary_str = sess.run([d_optim, d_sum], 
                                      feed_dict = {images: batch_images, 
                                                   z: batch_z, 
                                                   y: batch_labels})
            writer.add_summary(summary_str, idx+1)

            # 更新 G 的参数
            _, summary_str = sess.run([g_optim, g_sum], 
                                      feed_dict = {z: batch_z, 
                                                   y: batch_labels})
            writer.add_summary(summary_str, idx+1)

            # 更新两次 G 的参数确保网络的稳定
            _, summary_str = sess.run([g_optim, g_sum], 
                                      feed_dict = {z: batch_z,
                                                   y: batch_labels})
            writer.add_summary(summary_str, idx+1)
            
            # 计算训练过程中的损失，打印出来
            errD_fake = d_loss_fake.eval({z: batch_z, y: batch_labels})
            errD_real = d_loss_real.eval({images: batch_images, y: batch_labels})
            errG = g_loss.eval({z: batch_z, y: batch_labels})

            if idx % 20 == 0:
                print("Epoch: [%2d] [%4d/%4d] d_loss: %.8f, g_loss: %.8f" \
                        % (epoch, idx, batch_idxs, errD_fake+errD_real, errG))
            
            # 训练过程中，用采样器采样，并且保存采样的图片到 
            # /home/your_name/TensorFlow/DCGAN/samples/
            if idx % 100 == 1:
                sample = sess.run(samples, feed_dict = {z: sample_z, y: sample_labels})
                samples_path = '/home/jackie/workspace/gan/dcgan/samples/'
                save_images(sample, [8, 8], 
                            samples_path + 'test_%d_epoch_%d.png' % (epoch, idx))
                print 'save down'
            
            # 每过 500 次迭代，保存一次模型
            if idx % 500 == 2:
                checkpoint_path = os.path.join(train_dir, 'DCGAN_model.ckpt')
                saver.save(sess, checkpoint_path, global_step = idx+1)
                
    sess.close()

if __name__ == '__main__':
    train() 
